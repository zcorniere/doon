#include "Logger.hpp"
#include "MapManager.hpp"
#include "ObjectManager.hpp"
#include "RenderManager.hpp"
#include "StorageManager.hpp"
#include "ThreadManager.hpp"

ThreadManager *thread_manager = nullptr;
Logger *logger = nullptr;
StorageManager *storage_manager = nullptr;
MapManager *map_manager = nullptr;
ObjectManager *object_manager = nullptr;
RenderManager *render_manager = nullptr;

__attribute__((constructor)) void init()
{
    logger = new Logger();
    thread_manager = new ThreadManager();
    storage_manager = new StorageManager();
    map_manager = new MapManager();
    object_manager = new ObjectManager();
    render_manager = new RenderManager();
}

__attribute__((destructor)) void stop()
{
    delete render_manager;
    delete object_manager;
    delete map_manager;
    delete storage_manager;
    delete thread_manager;
    delete logger;
}
