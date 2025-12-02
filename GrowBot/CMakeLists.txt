cmake_minimum_required(VERSION 4.0)
project(GrowBot)

set(CMAKE_CXX_STANDARD 20)

add_executable(GrowBot main.cpp
        Database.hpp
        Database.cpp)

target_include_directories(GrowBot PRIVATE /usr/include/dpp)
target_link_libraries(GrowBot PRIVATE dpp ssl crypto curl z zstd)