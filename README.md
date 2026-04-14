## Laboratory work V

Данная лабораторная работа посвещена изучению фреймворков для тестирования на примере **GTest**

```sh
$ open https://github.com/google/googletest
```

## Tutorial

Настройка окружения 
```sh
$ export GITHUB_USERNAME=<имя_пользователя>
$ alias gsed=sed # for *-nix system
$ cd ${GITHUB_USERNAME}/workspace
$ pushd .
$ source scripts/activate
```
Клонируем предыдущую работу и меняем remote на новый репозиторий 
```sh
$ git clone https://github.com/${GITHUB_USERNAME}/lab04 projects/lab05
$ cd projects/lab05
$ git remote remove origin
$ git remote add origin https://github.com/${GITHUB_USERNAME}/lab05
```
Добавляем GoogleTest как git submodule для использования в тестах
```sh
$ mkdir third-party
$ git submodule add https://github.com/google/googletest third-party/gtest
```
```sh
Cloning into '/home/amir/ziatdinovamir/workspace/projects/lab05/third-party/gtest'...
remote: Enumerating objects: 28616, done.
remote: Counting objects: 100% (54/54), done.
remote: Compressing objects: 100% (38/38), done.
remote: Total 28616 (delta 28), reused 16 (delta 16), pack-reused 28562 (from 2)
Receiving objects: 100% (28616/28616), 13.71 MiB | 4.87 MiB/s, done.
Resolving deltas: 100% (21259/21259), done.
```
```sh
$ cd third-party/gtest && git checkout release-1.8.1 && cd ../..
```
```sh
You are in 'detached HEAD' state. You can look around, make experimental
changes and commit them, and you can discard any commits you make in this
state without impacting any branches by switching back to a branch.

If you want to create a new branch to retain commits you create, you may
do so (now or later) by using -c with the switch command. Example:

  git switch -c <new-branch-name>

Or undo this operation with:

  git switch -

Turn off this advice by setting config variable advice.detachedHead to false

HEAD is now at 2fe3bd99 Merge pull request #1433 from dsacre/fix-clang-warnings
```
```sh
$ git add third-party/gtest
$ git commit -m"added gtest framework"
```
Редактируем CMakeLists.txt, добавляем опцию BUILD_TESTS и конфигурацию для сборки тестов
```sh
$ gsed -i '/option(BUILD_EXAMPLES "Build examples" OFF)/a\
option(BUILD_TESTS "Build tests" OFF)
' CMakeLists.txt
$ cat >> CMakeLists.txt <<EOF

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB \${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(check \${\${PROJECT_NAME}_TEST_SOURCES})
  target_link_libraries(check \${PROJECT_NAME} gtest_main)
  add_test(NAME check COMMAND check)
endif()
EOF
```
Создание теста
```sh
$ mkdir tests
$ cat > tests/test1.cpp <<EOF
#include <print.hpp>

#include <gtest/gtest.h>

TEST(Print, InFileStream)
{
  std::string filepath = "file.txt";
  std::string text = "hello";
  std::ofstream out{filepath};

  print(text, out);
  out.close();

  std::string result;
  std::ifstream in{filepath};
  in >> result;

  EXPECT_EQ(result, text);
}
EOF
```
Сборка и запуск теста
```sh
$ cmake -H. -B_build -DBUILD_TESTS=ON
$ cmake --build _build
```
```sh
[  8%] Building CXX object CMakeFiles/print.dir/src/print.cpp.o
[ 16%] Linking CXX static library libprint.a
[ 16%] Built target print
[ 25%] Building CXX object third-party/gtest/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
[ 33%] Linking CXX static library ../../../lib/libgtest.a
[ 33%] Built target gtest
[ 41%] Building CXX object third-party/gtest/googletest/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
[ 50%] Linking CXX static library ../../../lib/libgtest_main.a
[ 50%] Built target gtest_main
[ 58%] Building CXX object CMakeFiles/check.dir/tests/test1.cpp.o
[ 66%] Linking CXX executable check
[ 66%] Built target check
[ 75%] Building CXX object third-party/gtest/googlemock/CMakeFiles/gmock.dir/src/gmock-all.cc.o
[ 83%] Linking CXX static library ../../../lib/libgmock.a
[ 83%] Built target gmock
[ 91%] Building CXX object third-party/gtest/googlemock/CMakeFiles/gmock_main.dir/src/gmock_main.cc.o
[100%] Linking CXX static library ../../../lib/libgmock_main.a
[100%] Built target gmock_main
```
```sh
$ cmake --build _build --target test
$ _build/check
$ cmake --build _build --target test -- ARGS=--verbose
```
```sh
Running main() from /home/amir/ziatdinovamir/workspace/projects/lab05/third-party/gtest/googletest/src/gtest_main.cc
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from Print
[ RUN      ] Print.InFileStream
[       OK ] Print.InFileStream (0 ms)
[----------] 1 test from Print (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```
```sh
cmake --build _build --target test -- ARGS=--verbose
```
```
Running tests...
UpdateCTestConfiguration  from :/home/amir/ziatdinovamir/workspace/projects/lab05/_build/DartConfiguration.tcl
UpdateCTestConfiguration  from :/home/amir/ziatdinovamir/workspace/projects/lab05/_build/DartConfiguration.tcl
Test project /home/amir/ziatdinovamir/workspace/projects/lab05/_build
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: check

1: Test command: /home/amir/ziatdinovamir/workspace/projects/lab05/_build/check
1: Working Directory: /home/amir/ziatdinovamir/workspace/projects/lab05/_build
1: Test timeout computed to be: 10000000
1: Running main() from /home/amir/ziatdinovamir/workspace/projects/lab05/third-party/gtest/googletest/src/gtest_main.cc
1: [==========] Running 1 test from 1 test suite.
1: [----------] Global test environment set-up.
1: [----------] 1 test from Print
1: [ RUN      ] Print.InFileStream
1: [       OK ] Print.InFileStream (0 ms)
1: [----------] 1 test from Print (0 ms total)
1: 
1: [----------] Global test environment tear-down
1: [==========] 1 test from 1 test suite ran. (0 ms total)
1: [  PASSED  ] 1 test.
1/1 Test #1: check ............................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```
Настройка GitHub Actions (вместо Travis CI)
```sh
mkdir -p .github/workflows
```
```sh
cat > .github/workflows/ci.yml <<'EOF'
name: CI

on:
  push:
    branches: [ main, master ]
  pull_request:
    branches: [ main, master ]

jobs:
  build-and-test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, windows-latest]
        build_type: [Debug, Release]
    
    steps:
    - uses: actions/checkout@v4
      with:
        submodules: true
    
    - name: Configure CMake (Linux)
      if: runner.os == 'Linux'
      run: cmake -B build -DCMAKE_BUILD_TYPE=${{ matrix.build_type }} -DBUILD_TESTS=ON
    
    - name: Configure CMake (Windows)
      if: runner.os == 'Windows'
      run: |
        cmake -B build -DCMAKE_BUILD_TYPE=${{ matrix.build_type }} -DBUILD_TESTS=ON
    
    - name: Build (Linux)
      if: runner.os == 'Linux'
      run: cmake --build build
    
    - name: Build (Windows)
      if: runner.os == 'Windows'
      run: cmake --build build --config ${{ matrix.build_type }}
    
    - name: Test (Linux)
      if: runner.os == 'Linux'
      working-directory: build
      run: |
        ctest --output-on-failure
        ./check
    
    - name: Test (Windows)
      if: runner.os == 'Windows'
      working-directory: build
      run: |
        ctest -C ${{ matrix.build_type }} --output-on-failure
        if (Test-Path "check.exe") { ./check.exe }
        elseif (Test-Path "${{ matrix.build_type }}/check.exe") { ./${{ matrix.build_type }}/check.exe }
        else { Write-Host "Check executable not found" }
      shell: pwsh
EOF
```
Создаем CMakeLists.txt 
```sh
cat > CMakeLists.txt <<'EOF'
cmake_minimum_required(VERSION 3.14)
project(print)

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
EOF
```
Коммитим изменения и пушим на GitHub
```sh
git add .
git commit -m "Attempt 4"
git push origin main
```
Делаем скриншот 
```sh
$ mkdir artifacts
$ sleep 20s && gnome-screenshot --file artifacts/screenshot.png
# for macOS: $ screencapture -T 20 artifacts/screenshot.png
# open https://github.com/${GITHUB_USERNAME}/lab05
```
![i8B2nKKNmaqDLG2ud5PTEAtrU8xYle-UAt9fyJq5bFRl0I2jzBTpoqN4WEVUcSZLkJ39TM1dAoL79bGhv1vwB-CU](https://github.com/user-attachments/assets/0c39860a-f4ef-439d-834a-df1b9c2c84de)

## Report

```sh
$ popd
$ export LAB_NUMBER=05
$ git clone https://github.com/tp-labs/lab${LAB_NUMBER} tasks/lab${LAB_NUMBER}
$ mkdir reports/lab${LAB_NUMBER}
$ cp tasks/lab${LAB_NUMBER}/README.md reports/lab${LAB_NUMBER}/REPORT.md
$ cd reports/lab${LAB_NUMBER}
$ edit REPORT.md
$ gist REPORT.md
```
# lab06
# lab06
