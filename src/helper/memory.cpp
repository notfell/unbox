#include "../vendor.hpp"

DWORD memory::get_pid(const char* name) {
    DWORD pid = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry{ sizeof(PROCESSENTRY32) };

    if (Process32First(snap, &entry))
        while (Process32Next(snap, &entry))
            if (!strcmp(entry.szExeFile, name))
                pid = entry.th32ProcessID;

    CloseHandle(snap);
    return pid;
}