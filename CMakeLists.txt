project(kgraphviewer)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)

# are these two really required ?
find_package(ZLIB REQUIRED)
if (APPLE)
   find_package(Carbon REQUIRED)
endif (APPLE)
find_package(KdepimLibs REQUIRED)
#macro_optional_find_package(GraphViz)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DQT3_SUPPORT)

remove_definitions(-DQT_NO_STL) 

include_directories (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})


configure_file (config-kgraphviewer.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kgraphviewer.h )


MESSAGE(STATUS "remember to port kgraphviewer (dbus)")
add_subdirectory(src)