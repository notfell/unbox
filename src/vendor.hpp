#pragma once

#define VERSION "0.1"
#define APP_NAME \
    " _   _ _  _ ___   ___  _  _ \n" \
    "| | | | \\| | _ ) / _ \\( \\/ )\n" \
    "| |_| | .` | _ \\| (_) |>  < \n" \
    " \\___/|_|\\_|___/ \\___//_/\\_\\\n" \
    "\n" \
    "  v" VERSION " - what's in the box? everything.\n" \
    "  https://github.com/notfell/unbox\n"

extern bool g_verbose;

#define success(MSG, ...) printf("[+] "               MSG "\n", ##__VA_ARGS__)
#define info(MSG, ...) printf("[*] "               MSG "\n", ##__VA_ARGS__)
#define debug(MSG, ...) if (g_verbose) { printf("[debug] "               MSG "\n", ##__VA_ARGS__); }
#define error(MSG, ...) fprintf(stderr, "[!] "      MSG "\n", ##__VA_ARGS__)

#include <rpc.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>
#include <filesystem>
#include <fstream>

#include "./helper/singleton.hpp"
#include "./helper/memory.hpp"
#include "./core/dumper.hpp"