cmake_minimum_required(VERSION 3.1.0)
project(doon)
enable_language(CXX)

set(CMAKE_BUILD_TYPE Release)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_COMPILER clang++)

include_directories(include)
include_directories(engine/include)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(CONAN_COMPILER ${CONAN_SETTINGS_COMPILER})
set(CONAN_LIBCXX ${CONAN_SETTINGS_COMPILER_LIBCXX})
set(CONAN_COMPILER_VERSION ${CONAN_SETTINGS_COMPILER_VERSION})
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(engine)

add_executable(doon source/GameInstance.cpp
    source/main.cpp
    source/objects/Barrel.cpp
    source/objects/Fireball.cpp
    source/objects/Poggers.cpp
    source/Player.cpp
)

target_link_libraries(doon
    raycaster
    pthread
    CONAN_PKG::libpng
    CONAN_PKG::sfml
)