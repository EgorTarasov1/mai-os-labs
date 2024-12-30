cmake_minimum_required(VERSION 3.10)
project(DistributedSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/Xiaomi/Desktop/ll5-7/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_PREFIX_PATH "C:/Users/Xiaomi/Desktop/ll5-7/vcpkg/installed/x64-windows")

find_package(ZeroMQ CONFIG REQUIRED)

set(SOURCES
    main.cpp
    NodeManager.cpp
    ComputeNode.cpp
)

add_executable(DistributedSystem ${SOURCES})

target_link_libraries(DistributedSystem PRIVATE libzmq)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(DistributedSystem PRIVATE -Wall -Wextra -pedantic -pthread)
endif()