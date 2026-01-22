cmake_minimum_required(VERSION 3.16)
project(my_project_name LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(tests 
    test/test.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(tests)

add_executable(main
    src/main.cpp
    src/buffer.cpp)

target_include_directories(main
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)