project(kgraphviewer)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)
include(ECMInstallIcons)
# search packages used by KDE
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core DBus Widgets Svg PrintSupport)

find_package(Boost 1.36 REQUIRED)
find_package(GraphViz REQUIRED)

find_package(KF5 REQUIRED COMPONENTS
    CoreAddons
    DocTools
    Parts
    KIO
    KHtml
    WidgetsAddons
)

if(NOT DOT)
   MESSAGE(STATUS "dot executable from GraphViz not found. You will need it at runtime")
endif(NOT DOT)

add_definitions (${QT_DEFINITIONS})

remove_definitions(-DQT_NO_STL) 

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${graphviz_INCLUDE_DIRECTORIES}
    ${KDE_INSTALL_INCLUDEDIR}
    ${QT_INCLUDE_DIRS}
)

configure_file (config-kgraphviewer.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kgraphviewer.h )

add_subdirectory(src)
add_subdirectory(doc)

