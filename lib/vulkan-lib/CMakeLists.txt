cmake_minimum_required(VERSION 3.8)
project(vulak)

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} source/Window.cpp
                            source/Pipeline.cpp
                            source/Device.cpp
                            source/DeletionQueue.cpp
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE include/)
target_include_directories(${PROJECT_NAME} PRIVATE ../glfw/)
target_include_directories(${PROJECT_NAME} PRIVATE ../vma/)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw
                                              vk-bootstrap
                                              vma
)

