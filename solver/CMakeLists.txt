cmake_minimum_required(VERSION 3.10)
project(solver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/solver_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../formatter_ex_lib
)

add_subdirectory(solver_lib)

add_executable(solver equation.cpp)
target_link_libraries(solver solver_lib formatter formatter_ex)
