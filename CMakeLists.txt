cmake_minimum_required(VERSION 2.8)
project(DriveEnum)
include_directories(./include)
add_subdirectory(DriveEnumLib)
add_executable(example example.cpp)
target_link_libraries(example DriveEnumLib)
target_link_libraries(example setupapi.lib)


