cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project("efficient-autocomplete" VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# add_subdirectory(src/ModuleA)
# add_subdirectory(src/ModuleB)  # As you add more modules

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

# Link the modules
# target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ModuleA)
# target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ModuleB)
