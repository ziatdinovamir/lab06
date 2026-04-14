cmake_minimum_required(VERSION 3.10)
project(hello_world)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_ex_lib
)

add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world formatter_ex)
