add_subdirectory(logger-cpp/ EXCLUDE_FROM_ALL)


set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
add_subdirectory(SFML/ EXCLUDE_FROM_ALL)
