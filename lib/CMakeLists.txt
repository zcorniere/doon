add_subdirectory(glfw/ EXCLUDE_FROM_ALL)

add_subdirectory(vk-bootstrap/ EXCLUDE_FROM_ALL)

add_subdirectory(glm/ EXCLUDE_FROM_ALL)

add_library(vma vma/vk_mem_alloc.cpp)
target_compile_options(vma PRIVATE -Wno-everything)

add_subdirectory(vulkan-lib EXCLUDE_FROM_ALL)

