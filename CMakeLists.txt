cmake_minimum_required(VERSION 3.2.0)
project(nitroshare)

# Point CMake to the custom modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# It would be nice if CMake offered a constant for Linux
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX 1)
endif()

set(PROJECT_NAME "NitroShare")
set(PROJECT_DESCRIPTION "Cross-platform network file transfer application")
set(PROJECT_AUTHOR "Nathan Osman")
set(PROJECT_URL "https://nitroshare.net")

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 4)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# At the very minimum, Qt 5.4+ is required
find_package(Qt5Network 5.4 REQUIRED)
find_package(Qt5Widgets 5.4)

# CLI and client are always available, UI is available if Qt5Widgets is available
option(BUILD_CLI "Build the NitroShare CLI app" ON)
option(BUILD_CLIENT "Build the NitroShare client app" ON)
if(Qt5Widgets_FOUND)
    option(BUILD_UI "Build the NitroShare UI app" ON)
endif()

# Allow file installation directories to be customized
set(INSTALL_BIN_PATH bin CACHE STRING "Application installation directory")
set(INSTALL_LIB_PATH lib CACHE STRING "Shared library installation directory")
set(INSTALL_PLUGIN_PATH "${INSTALL_LIB_PATH}/nitroshare/plugins" CACHE STRING "Plugin installation directory")

# Output paths mirror the installation paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/${INSTALL_BIN_PATH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/${INSTALL_LIB_PATH}")
set(PLUGIN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/${INSTALL_PLUGIN_PATH}")

# Test suite is always disabled by default
option(BUILD_TESTS "Build test suite" OFF)
if(BUILD_TESTS)
    enable_testing()
    find_package(Qt5Test 5.4 REQUIRED)
endif()

# Enable local API if QHttpEngine is available
find_package(qhttpengine 1.0.0)
if(qhttpengine_FOUND)
    option(BUILD_API "Build local API plugin" ON)
endif()

# Enable mDNS discovery if QMdnsEngine is available
find_package(qmdnsengine)
if(qmdnsengine_FOUND)
    option(BUILD_MDNS "Build mDNS discovery plugin" ON)
endif()

# Enable indicator if GTK, appindicator, and libnotify are present
if(LINUX)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(appindicator gtk+-2.0 appindicator-0.1 libnotify)
        if(appindicator_FOUND)
            option(BUILD_INDICATOR "Build application indicator" ON)
        endif()
    endif()
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# The library and plugins are always built
add_subdirectory(libnitroshare)
add_subdirectory(plugins)

if(BUILD_CLI)
    add_subdirectory(cli)
endif()

if(BUILD_CLIENT)
    add_subdirectory(client)
endif()

if(BUILD_UI)
    add_subdirectory(ui)
endif()

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_EXECUTABLES nitroshare-ui;NitroShare)

include(CPack)
