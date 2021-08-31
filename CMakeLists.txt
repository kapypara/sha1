cmake_minimum_required(VERSION 3.20)
project(my_sha1)

set(CMAKE_CXX_STANDARD 14)

add_executable(my_sha1 main.cpp bit_tools.h bit_tools.cpp sha1.cpp sha1.h)
