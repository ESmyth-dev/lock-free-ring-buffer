cmake_minimum_required(VERSION 3.16)
project(my_project_name LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(my_executable
    src/main.cpp
    src/buffer.cpp)

target_include_directories(my_executable
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)