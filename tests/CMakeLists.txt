cmake_minimum_required(VERSION 3.11)
project("generator")
set(CMAKE_CXX_STANDARD 20)

set(SRCS src/generator.cpp)
add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC "include")
