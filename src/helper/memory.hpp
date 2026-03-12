#pragma once

#include "singleton.hpp"
#include <Windows.h>

class memory : public singleton<memory> {
    public:
        DWORD get_pid(const char* name);
};