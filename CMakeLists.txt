cmake_minimum_required(VERSION 3.8)
project(doon)
include(shaders/CMakeRule.txt)

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(engine)
add_executable(${PROJECT_NAME} source/GameInstance.cpp
                               source/main.cpp
                               source/objects/Barrel.cpp
                               source/objects/Fireball.cpp
                               source/objects/Poggers.cpp
                               source/Player.cpp
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

add_shader(${PROJECT_NAME} simple_shader.vert)
add_shader(${PROJECT_NAME} simple_shader.frag)

target_include_directories(${PROJECT_NAME} PRIVATE include/)
target_include_directories(${PROJECT_NAME} PRIVATE engine/include/)
target_include_directories(${PROJECT_NAME} PRIVATE lib/vulkan-lib/include/)
target_include_directories(${PROJECT_NAME} PRIVATE lib/glfw/)
target_include_directories(${PROJECT_NAME} PRIVATE lib/glm/)
target_include_directories(${PROJECT_NAME} PRIVATE lib/vma/)
target_include_directories(${PROJECT_NAME} PRIVATE lib/stb/)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE raycaster)
#target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics
#                                              sfml-window
#                                              sfml-system)

add_subdirectory(lib)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw
                                              vk-bootstrap
                                              glm
                                              vma
                                              png
                                              vulak
)

