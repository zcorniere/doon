cmake_minimum_required(VERSION 3.8)
project(doon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Looking for lld")
find_program(LLD_FOUND lld)

if (LLD_FOUND)
    message(STATUS "Looking for lld - found")
    set(CMAKE_C_LINKER_FLAGS "-fuse-ld=lld")
    set(CMAKE_CXX_LINKER_FLAGS "-fuse-ld=lld")
    set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")
else()
    message(STATUS "Looking for lld - not found")
endif()

add_subdirectory(lib)
add_subdirectory(engine)

add_executable(${PROJECT_NAME} source/GameInstance.cpp
                               source/main.cpp
                               source/objects/Barrel.cpp
                               source/objects/Fireball.cpp
                               source/objects/Poggers.cpp
                               source/Player.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include/)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raycaster logger)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics
                                              sfml-window
                                              sfml-system
                                              png
                                              pthread
)

