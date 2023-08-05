cmake_minimum_required(VERSION 3.20)

# Project name
project("Skybox")

# Set C++ Required version
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)

# Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings -Wno-parentheses -Wfloat-equal -pedantic ${CMAKE_CXX_FLAGS}")

# Use -std=c++XX instead of -std=gnu++XX
set(CMAKE_CXX_EXTENSIONS OFF)

# Set dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/third_party)
execute_process(
    COMMAND git clone "https://github.com/bschiffthaler/BSlogger.git" BSlogger
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/third_party)
execute_process(
    COMMAND git clone "https://github.com/nothings/stb.git" stb
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/third_party)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/third_party/BSlogger/src ${CMAKE_CURRENT_BINARY_DIR}/third_party/stb)

file(GLOB SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(lib ${SRC})
target_include_directories(lib PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Set the main source to generate the executable code
add_executable(main main.cpp)

target_link_libraries(main GL GLEW glfw assimp lib)
