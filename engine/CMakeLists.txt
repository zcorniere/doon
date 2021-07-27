add_library(raycaster source/lib.cpp
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

target_include_directories(raycaster PUBLIC include/)
target_link_libraries(raycaster PUBLIC logger)
