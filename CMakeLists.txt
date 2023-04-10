cmake_minimum_required(VERSION 3.25)
project(TestECS)

set(CMAKE_CXX_STANDARD 20)

file(
        GLOB_RECURSE
        source_files
        src/*
)

include_directories(src)

add_executable(TestECS ${source_files})
