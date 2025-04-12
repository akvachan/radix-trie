cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project("radix-trie" VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Add executable and set the include directories
add_executable(${CMAKE_PROJECT_NAME} main.cpp) 
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src/)
