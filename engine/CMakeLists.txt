add_library(raycaster STATIC source/lib.cpp
                      source/DepthBuffer.cpp
                      source/ObjectManager.cpp
                      source/Frame.cpp
                      source/RenderManager.cpp
                      source/Ray.cpp
                      source/StorageManager.cpp
                      source/Map.cpp
                      source/MapManager.cpp
                      source/Loader.cpp
                      source/ThreadManager.cpp
)

target_compile_definitions(raycaster PRIVATE LOGGER_EXTERN_DECLARATION_PTR)
target_include_directories(raycaster PUBLIC include/)
target_link_libraries(raycaster PUBLIC logger)
