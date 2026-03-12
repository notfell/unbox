cmake_minimum_required(VERSION 3.20)
project(unbox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(unbox
    src/helper/singleton.hpp

    src/main.cpp 
    src/vendor.hpp

    src/core/dumper.cpp
    src/core/dumper.hpp

    src/helper/memory.cpp
    src/helper/memory.hpp
)
target_link_libraries(unbox PRIVATE kernel32 user32)