include_directories(include)

add_library(raycaster STATIC source/lib.cpp
    source/DepthBuffer.cpp
    source/ObjectManager.cpp
    source/Frame.cpp
    source/RenderManager.cpp
    source/Ray.cpp
    source/StorageManager.cpp
    source/Map.cpp
    source/MapManager.cpp
    source/Logger.cpp
    source/Loader.cpp
    source/ThreadManager.cpp
)
