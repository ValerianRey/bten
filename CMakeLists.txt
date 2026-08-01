cmake_minimum_required(VERSION 3.16)
project(bten CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bten
    mapping.cpp
    tensor.cpp
    test.cpp
)

target_compile_options(bten PRIVATE -Wall -Wextra)
