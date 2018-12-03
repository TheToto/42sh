cmake_minimum_required(VERSION 3.0)
project (42sh)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -std=c99 -pedantic -fsanitize=address")
set(CMAKE_C_STANDARD_LIBRARIES "-lreadline")
include_directories(src/include)
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(42sh ${SOURCES})
add_custom_target(check "../test/test.sh" "check")
