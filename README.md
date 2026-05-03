## Laboratory work VI

Данная лабораторная работа посвещена изучению средств пакетирования на примере **CPack**

```sh
$ open https://cmake.org/Wiki/CMake:CPackPackageGenerators
```

## Tutorial

Настраиваем окружение 
```sh
$ export GITHUB_USERNAME=<имя_пользователя>
$ export GITHUB_EMAIL=<адрес_почтового_ящика>
$ alias edit=<nano|vi|vim|subl>
$ alias gsed=sed # for *-nix system
```

```sh
$ cd ${GITHUB_USERNAME}/workspace
$ pushd .
$ source scripts/activate
```
Клонируем репозиторий лабораторной работы №5
```sh
$ git clone https://github.com/${GITHUB_USERNAME}/lab05 projects/lab06
$ cd projects/lab06
$ git remote remove origin
$ git remote add origin https://github.com/${GITHUB_USERNAME}/lab06
```
Добавление версионирования в `CMakeLists.txt`
```sh
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_STRING "v\${PRINT_VERSION}")
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION\
  \${PRINT_VERSION_MAJOR}.\${PRINT_VERSION_MINOR}.\${PRINT_VERSION_PATCH}.\${PRINT_VERSION_TWEAK})
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_TWEAK 0)
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_PATCH 0)
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_MINOR 1)
' CMakeLists.txt
$ gsed -i '/project(print)/a\
set(PRINT_VERSION_MAJOR 0)
' CMakeLists.txt
```
```sh
$ git diff
```
```sh
diff --git a/CMakeLists.txt b/CMakeLists.txt
index ec70034..d73a3ae 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -1,5 +1,13 @@
 cmake_minimum_required(VERSION 3.14)
 project(print)
+set(PRINT_VERSION_MAJOR 0)
+set(PRINT_VERSION_MINOR 1)
+set(PRINT_VERSION_PATCH 0)
+set(PRINT_VERSION_TWEAK 0)
+set(PRINT_VERSION
+  ${PRINT_VERSION_MAJOR}.${PRINT_VERSION_MINOR}.${PRINT_VERSION_PATCH}.${PRINT_VERSION_TWEAK})
+set(PRINT_VERSION_STRING "v${PRINT_VERSION}")
+set(PRINT_VERSION_STRING "v${PRINT_VERSION}")
 
 set(CMAKE_CXX_STANDARD 17)
 set(CMAKE_CXX_STANDARD_REQUIRED ON)
```
Создание файлов описания
```sh
$ touch DESCRIPTION && edit DESCRIPTION
$ touch ChangeLog.md
$ export DATE="`LANG=en_US date +'%a %b %d %Y'`"
$ cat > ChangeLog.md <<EOF
* ${DATE} ${GITHUB_USERNAME} <${GITHUB_EMAIL}> 0.1.0.0
- Initial RPM release
EOF
```
Создание `CPackConfig.cmake`
```sh
$ cat > CPackConfig.cmake <<EOF
include(InstallRequiredSystemLibraries)
EOF
```

```sh
$ cat >> CPackConfig.cmake <<EOF
set(CPACK_PACKAGE_CONTACT ${GITHUB_EMAIL})
set(CPACK_PACKAGE_VERSION_MAJOR \${PRINT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR \${PRINT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH \${PRINT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK \${PRINT_VERSION_TWEAK})
set(CPACK_PACKAGE_VERSION \${PRINT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_FILE \${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "static C++ library for printing")
EOF
```

```sh
$ cat >> CPackConfig.cmake <<EOF

set(CPACK_RESOURCE_FILE_LICENSE \${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README \${CMAKE_CURRENT_SOURCE_DIR}/README.md)
EOF
```

```sh
$ cat >> CPackConfig.cmake <<EOF

set(CPACK_RPM_PACKAGE_NAME "print-devel")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "print")
set(CPACK_RPM_CHANGELOG_FILE \${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.md)
set(CPACK_RPM_PACKAGE_RELEASE 1)
EOF
```

```sh
$ cat >> CPackConfig.cmake <<EOF

set(CPACK_DEBIAN_PACKAGE_NAME "libprint-dev")
set(CPACK_DEBIAN_PACKAGE_PREDEPENDS "cmake >= 3.0")
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)
EOF
```

```sh
$ cat >> CPackConfig.cmake <<EOF

include(CPack)
EOF
```
Подключение `CPack` и обновление README
```sh
$ cat >> CMakeLists.txt <<EOF

include(CPackConfig.cmake)
EOF
```

```sh
$ gsed -i 's/lab05/lab06/g' README.md
```
Коммитим и отправляем на GitHub, добавляя теги
```sh
$ git add .
$ git commit -m"added cpack config"
$ git tag v0.1.0.0
$ git push origin master --tags
```

```sh
$ cmake -H. -B_build
```
```sh
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/amir/ziatdinovamir/workspace/projects/lab06/_build
```
```sh
$ cmake --build _build
```
```sh
[ 50%] Building CXX object CMakeFiles/print.dir/sources/print.cpp.o
[100%] Linking CXX static library libprint.a
[100%] Built target print
```
```sh
$ cd _build
$ cpack -G "TGZ"
```
```sh
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: print
CPack: - Install project: print []
CPack: Create package
CPack: - package: /home/amir/ziatdinovamir/workspace/projects/lab06/_build/print-0.1.0.0-Linux.tar.gz generated.
```
```sh
$ cd ..
```
```sh
$ cmake -H. -B_build -DCPACK_GENERATOR="TGZ"
```
```sh
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/amir/ziatdinovamir/workspace/projects/lab06/_build
```
```sh
$ cmake --build _build --target package
```
```sh
[100%] Built target print
Run CPack packaging tool...
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: print
CPack: - Install project: print []
CPack: Create package
CPack: - package: /home/amir/ziatdinovamir/workspace/projects/lab06/_build/print-0.1.0.0-Linux.tar.gz generated.
```
```sh
$ mkdir artifacts
$ mv _build/*.tar.gz artifacts
$ tree artifacts
```
```sh
artifacts
└── print-0.1.0.0-Linux.tar.gz
```
## Homework

После того, как вы настроили взаимодействие с системой непрерывной интеграции,</br>
обеспечив автоматическую сборку и тестирование ваших изменений, стоит задуматься</br>
о создание пакетов для измениний, которые помечаются тэгами (см. вкладку [releases](https://github.com/tp-labs/lab06/releases)).</br>
Пакет должен содержать приложение _solver_ из [предыдущего задания](https://github.com/tp-labs/lab03#задание-1)
Таким образом, каждый новый релиз будет состоять из следующих компонентов:
- архивы с файлами исходного кода (`.tar.gz`, `.zip`)
- пакеты с бинарным файлом _solver_ (`.deb`, `.rpm`, `.msi`, `.dmg`)

Создаем `CMakeLists.txt`
```sh
cat > CMakeLists.txt <<'EOF'
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

add_subdirectory(formatter_lib)
add_subdirectory(formatter_ex_lib)
add_subdirectory(hello_world)
add_subdirectory(solver)

include(CPackConfig.cmake)
EOF
```
Создаем `CPackConfig.cmake`
```sh
cat > CPackConfig.cmake <<'EOF'
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_CONTACT "ziatdinov.amir.07@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Amir Ziatdinov <ziatdinov.amir.07@gmail.com>")

set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_VERSION_TWEAK 0)
set(CPACK_PACKAGE_VERSION "0.1.0.0")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Solver application - solves mathematical equations")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION)

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.rtf)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

set(CPACK_PACKAGE_NAME "solver")

set(CPACK_DEBIAN_PACKAGE_NAME "solver")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++6")
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)

set(CPACK_RPM_PACKAGE_NAME "solver")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_RELEASE 1)

set(CPACK_DMG_VOLUME_NAME "Solver ${CPACK_PACKAGE_VERSION}")
set(CPACK_DMG_FORMAT "UDBZ")

set(CPACK_WIX_LICENSE_RTF ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.rtf)
set(CPACK_WIX_UPGRADE_GUID "12345678-1234-1234-1234-123456789012")
set(CPACK_WIX_PRODUCT_NAME "Solver")
set(CPACK_WIX_MANUFACTURER "Amir Ziatdinov")
set(CPACK_WIX_PACKAGE_LANGUAGES "en-US")

include(CPack)
EOF
```
Подключаем `CPackConfig.cmake` в `CMakeLists.txt`
```sh
echo "" >> CMakeLists.txt
echo "include(CPackConfig.cmake)" >> CMakeLists.txt
```
Cоздаем `workflows/ci.yml`
```sh
cat > .github/workflows/ci.yml <<'EOF'
name: CI Build and Package

on:
  push:
    branches: [ main ]
    tags: [ 'v*' ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    name: ${{ matrix.os }} - ${{ matrix.build_type }}
    runs-on: ${{ matrix.os }}
    
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        build_type: [Release, Debug]
        include:
          - os: ubuntu-latest
            build_type: Release
            cpack_generator: "TGZ;DEB;RPM"
          - os: ubuntu-latest
            build_type: Debug
            cpack_generator: "TGZ"
          - os: macos-latest
            build_type: Release
            cpack_generator: "DragNDrop;TGZ"
          - os: macos-latest
            build_type: Debug
            cpack_generator: "TGZ"
          - os: windows-latest
            build_type: Release
            cpack_generator: "WIX;ZIP"
          - os: windows-latest
            build_type: Debug
            cpack_generator: "ZIP"
    
    steps:
    - name: Checkout code
      uses: actions/checkout@v4
      with:
        fetch-depth: 0
    
    - name: Setup Ubuntu
      if: matrix.os == 'ubuntu-latest'
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake build-essential rpm
      shell: bash
    
    - name: Setup macOS
      if: matrix.os == 'macos-latest'
      run: |
        brew install cmake
      shell: bash
    
    - name: Setup Windows
      if: matrix.os == 'windows-latest'
      run: |
        cmake --version
        echo "WiX Toolset is pre-installed"
      shell: powershell
    
    - name: Configure CMake
      run: cmake -H. -B_build -DCMAKE_BUILD_TYPE=${{ matrix.build_type }}
      shell: bash
    
    - name: Build
      run: cmake --build _build --config ${{ matrix.build_type }}
      shell: bash
    
    - name: Create packages
      run: |
        cd _build
        cpack -G "${{ matrix.cpack_generator }}"
      shell: bash
    
    - name: Upload artifacts
      uses: actions/upload-artifact@v4
      with:
        name: ${{ matrix.os }}-${{ matrix.build_type }}
        path: |
          _build/*.tar.gz
          _build/*.deb
          _build/*.rpm
          _build/*.dmg
          _build/*.zip
          _build/*.msi
          _build/*.exe
      continue-on-error: true
    
    - name: Create Release
      if: startsWith(github.ref, 'refs/tags/') && matrix.os == 'ubuntu-latest' && matrix.build_type == 'Release'
      uses: softprops/action-gh-release@v1
      with:
        files: |
          _build/*.tar.gz
          _build/*.deb
          _build/*.rpm
          _build/*.dmg
          _build/*.zip
          _build/*.msi
          _build/*.exe
        generate_release_notes: true
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
EOF
```
```sh
cmake -H. -B_build -DCMAKE_BUILD_TYPE=Release
```
```sh
-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.4s)
-- Generating done (0.0s)
-- Build files have been written to: /home/amir/ziatdinovamir/workspace/projects/lab06/_build
```
```sh
cmake --build _build
```
```sh
[ 50%] Building CXX object CMakeFiles/print.dir/sources/print.cpp.o
[100%] Linking CXX static library libprint.a
[100%] Built target print
```
```sh
cd _build
cpack -G "TGZ"
```
```sh
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: print
CPack: - Install project: print []
CPack: Create package
CPack: - package: /home/amir/ziatdinovamir/workspace/projects/lab06/_build/solver-0.1.0.0-Linux.tar.gz generated.
```
```sh
cpack -G "DEB"
```
```sh
CPack: Create package using DEB
CPack: Install projects
CPack: - Run preinstall target for: print
CPack: - Install project: print []
CPack: Create package
CPack: - package: /home/amir/ziatdinovamir/workspace/projects/lab06/_build/solver-0.1.0.0-Linux.deb generated.
```
Добавить все изменения в Git
```sh
git add .
git commit -m "Fix MSI2"
git push origin main
```








