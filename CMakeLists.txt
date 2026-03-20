cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 23)
project("result_algh")

set(SRCS src/test.cpp)
add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

