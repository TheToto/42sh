cmake_minimum_required(VERSION 3.0)
project (42sh)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -std=c99 -pedantic -fsanitize=address")
set(CMAKE_C_STANDARD_LIBRARIES "-lreadline")
include_directories(src/include)
file(GLOB SOURCES
        "src/*.c"
        "src/env/*.c"
        "src/parser/*.c"
        "src/parser/ast/*.c")

add_executable(42sh ${SOURCES})
