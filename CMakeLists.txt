cmake_minimum_required(VERSION 3.11)
project("range_query")
set(CMAKE_CXX_STANDARD 20)


set(SRCS src/main.cpp)
add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(tests)
add_subdirectory(comparison)