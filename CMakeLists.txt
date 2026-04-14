cmake_minimum_required(VERSION 3.14)
project(print)
set(PRINT_VERSION_MAJOR 0)
set(PRINT_VERSION_MINOR 1)
set(PRINT_VERSION_PATCH 0)
set(PRINT_VERSION_TWEAK 0)
set(PRINT_VERSION
  ${PRINT_VERSION_MAJOR}.${PRINT_VERSION_MINOR}.${PRINT_VERSION_PATCH}.${PRINT_VERSION_TWEAK})
set(PRINT_VERSION_STRING "v${PRINT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    foreach(flag_var 
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
        if(${flag_var} MATCHES "/MT")
            string(REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

add_library(print STATIC sources/print.cpp)

target_include_directories(print PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(BUILD_EXAMPLES)
    add_executable(example examples/example1.cpp)
    target_link_libraries(example PRIVATE print)
endif()

if(BUILD_TESTS)
    enable_testing()
    
    include(FetchContent)
    
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    
    FetchContent_MakeAvailable(googletest)
    
    add_executable(check tests/test1.cpp)
    
    target_link_libraries(check PRIVATE print gtest_main)
    
    target_include_directories(check PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    if(MSVC)
        target_link_options(check PRIVATE 
            /NODEFAULTLIB:libcmt.lib
            /NODEFAULTLIB:libcmtd.lib
        )
    endif()
    
    add_test(NAME check COMMAND check)
endif()

include(CPackConfig.cmake)
