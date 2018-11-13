cmake_minimum_required(VERSION 3.0)
project (42sh)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Werror -Wall -Wextra -std=c99 -pedantic")
include_directories(src/include)
file(GLOB SOURCES "src/*.c")

add_executable(42sh ${SOURCES})
