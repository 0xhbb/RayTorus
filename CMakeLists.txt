cmake_minimum_required(VERSION 3.20)
project(RayTorus CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

add_executable(RayTorus
        src/main.cpp
        src/Renderer.cpp
        src/Raytracer.cpp
        include/Renderer.h
)

# Crucial: Maps your /include directory to the compiler's search path
target_include_directories(RayTorus PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
