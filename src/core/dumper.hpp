#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class dumper {
public:
    dumper(const std::string& output_dir, const std::string& filter);
    

private:
    DWORD       m_pid{0};
    HANDLE      m_handle{nullptr};
    std::string m_output_dir;
    std::string m_filter;

    bool        initialize();
    void        dump();
    std::string find_name(BYTE* data, DWORD size);
};