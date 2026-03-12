#include "../vendor.hpp"
#include <fstream>
#include <cstdint>
#include <string>

dumper::dumper(const std::string& output_dir, const std::string& filter)
    : m_output_dir(output_dir), m_filter(filter)
{
    if (!initialize())
    {
        error("initialization failed");
        return;
    }

    dump();
}

bool dumper::initialize()
{
    m_pid = memory::get()->get_pid("sbox.exe");
    if (!m_pid)
    {
        error("sbox.exe not found");
        return false;
    }
    success("found sbox.exe [pid: %lu]", m_pid);

    m_handle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, m_pid);
    if (!m_handle)
    {
        error("OpenProcess failed [%lu]", GetLastError());
        return false;
    }
    success("process handle acquired [0x%p]", m_handle);

    return true;
}

void dumper::dump()
{
    std::filesystem::create_directories(m_output_dir);
    info("scanning memory...");

    MEMORY_BASIC_INFORMATION mbi{};
    BYTE*  addr  = nullptr;
    BYTE   header[0x1000]{};
    SIZE_T read  = 0;
    int    count = 0;

    while (VirtualQueryEx(m_handle, addr, &mbi, sizeof(mbi)))
    {
        addr = (BYTE*)mbi.BaseAddress + mbi.RegionSize;

        if (mbi.State != MEM_COMMIT || mbi.Type != MEM_MAPPED || mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
            continue;

        if (!ReadProcessMemory(m_handle, mbi.BaseAddress, header, sizeof(header), &read) || read < 0x200)
            continue;

        if (header[0] != 'M' || header[1] != 'Z')
            continue;

        auto* dos = (IMAGE_DOS_HEADER*)header;
        if (dos->e_lfanew <= 0 || dos->e_lfanew + 4 > (LONG)read)
            continue;

        if (*(DWORD*)(header + dos->e_lfanew) != IMAGE_NT_SIGNATURE)
            continue;

        auto* file_hdr = (IMAGE_FILE_HEADER*)(header + dos->e_lfanew + 4);
        auto* opt      = (BYTE*)(file_hdr + 1);
        WORD  magic    = *(WORD*)opt;

        DWORD image_size = 0;
        if (magic == 0x10B)
            image_size = ((IMAGE_OPTIONAL_HEADER32*)opt)->SizeOfImage;
        else if (magic == 0x20B)
            image_size = ((IMAGE_OPTIONAL_HEADER64*)opt)->SizeOfImage;
        else 
            continue;

        if (!image_size) continue;

        std::vector<BYTE> pe(image_size, 0);
        BYTE* base = (BYTE*)mbi.BaseAddress;

        for (DWORD offset = 0; offset < image_size; offset += 0x1000)
        {
            SIZE_T page_read = 0;
            ReadProcessMemory(m_handle, base + offset, pe.data() + offset, 0x1000, &page_read);
        }

        debug("pe @ 0x%llX", (uintptr_t)mbi.BaseAddress)

        std::string name = find_name(pe.data(), image_size);
        if (name.empty()) continue;

        char addr_hex[32];
        snprintf(addr_hex, sizeof(addr_hex), "%llX", (uintptr_t)mbi.BaseAddress);

        std::string base_name = m_output_dir + "\\" + name;
        std::string  path = base_name + " @ 0x" + addr_hex + ".unboxed";

        std::ofstream out(path, std::ios::binary);
        if (!out)
        {
            error("write failed: %s", path.c_str());
            continue;
        }

        out.write((char*)pe.data(), image_size);
        success("[%d] %s (0x%llX, %lu bytes)", ++count, name.c_str(), (uintptr_t)base, image_size);
    }

    info("done - %d dumped", count);
}

std::string dumper::find_name(BYTE* data, DWORD size)
{
    if (m_filter.empty()) return "";

    const char* filter     = m_filter.c_str();
    DWORD       filter_len = (DWORD)m_filter.size();

    for (DWORD i = 0; i + filter_len < size; i++)
    {
        if (memcmp(data + i, filter, filter_len) != 0) continue;

        DWORD end = i;
        bool found_package = false;

        while (end + 3 < size)
        {
            BYTE c = data[end];

            if (c <= 0x20 || c >= 0x7F)
                break;

            if (memcmp(data + end, ".dll", 4) == 0)
            {
                end += 4;
                found_package = true;
                break;
            }
            end++;
        }

        if (!found_package) continue;

        return std::string((char*)data + i, end - i);
    }

    return "";
}