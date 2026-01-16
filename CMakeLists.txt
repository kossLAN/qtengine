cmake_minimum_required(VERSION 3.16)

project(qtengine_plugin LANGUAGES CXX)

set(QT6_MIN_VERSION 6.9)
set(QT5_MIN_VERSION 5.15)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_POSITIONeINDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

option(BUILD_QT5 "Build with Qt5 support" ON)
option(BUILD_QT6 "Build with Qt6 support" ON)

# nix workaround
if (CMAKE_EXPORT_COMPILE_COMMANDS)
	set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

function(build_qtengine qt_major)
	if(NOT qt_major)
		set(qt_major 6)
	endif()

	if(NOT qt_major EQUAL 5 AND NOT qt_major EQUAL 6)
		message(FATAL_ERROR "build_qtengine() expects 5 or 6, got '${qt_major}'.")
	endif()

	if(qt_major EQUAL 5)
		set(qt_min_version "${QT5_MIN_VERSION}")
        set(plugin_dir ${QT5_PLUGINDIR})
		set(qtengine_name "qt5engine")
	else()
		set(qt_min_version "${QT6_MIN_VERSION}")
        set(plugin_dir "${QT6_PLUGINDIR}")
		set(qtengine_name "qt6engine")
	endif()

	set(QT_MAJOR "${qt_major}" CACHE STRING "Qt major version to build against (5 or 6)" FORCE)
	set(QT_MIN_VERSION "${qt_min_version}" CACHE STRING "Qt minimum version" FORCE)
	set(QT_NAMESPACE "Qt${qt_major}" CACHE STRING "Qt CMake target namespace" FORCE)
	set(QTENGINE_NAME "${qtengine_name}" CACHE STRING "Engine name based on Qt major" FORCE)
    set(PLUGINDIR "${plugin_dir}" CACHE STRING "Install location of the plugin libraries" FORCE)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

    if(NOT DEFINED PLUGINDIR OR PLUGINDIR STREQUAL "")
        set(PLUGINDIR "lib/qt-${QT_MAJOR}/plugins")
    endif()
    
    if(QT_MAJOR EQUAL 5)
        find_package(Qt5 ${QT5_MIN_VERSION} COMPONENTS Core Gui Widgets REQUIRED)
        find_package(Qt5Gui ${QT5_MIN_VERSION} CONFIG REQUIRED Private)
        find_package(Qt5ThemeSupport ${QT5_MIN_VERSION} CONFIG REQUIRED Private)

        find_package(KF5Config CONFIG REQUIRED)
        find_package(KF5ConfigWidgets CONFIG REQUIRED)
        find_package(KF5IconThemes CONFIG REQUIRED)

        add_subdirectory(src qt5engine)
    elseif(QT_MAJOR EQUAL 6)
        find_package(Qt6 ${QT6_MIN_VERSION} COMPONENTS Core Gui Widgets REQUIRED)

        find_package(KF6Config CONFIG REQUIRED)
        find_package(KF6ColorScheme CONFIG REQUIRED)
        find_package(KF6IconThemes CONFIG REQUIRED)

        # In Qt 6.10, private dependencies are required to be explicit,
        # but they could not be explicitly depended on prior to 6.9.
        if(QT_MAJOR EQUAL 6 AND Qt6_VERSION VERSION_GREATER_EQUAL "6.9.0")
            set(QT_NO_PRIVATE_MODULE_WARNING ON)
            find_package(Qt6 ${QT6_MIN_VERSION} COMPONENTS GuiPrivate REQUIRED)
        endif()

        add_subdirectory(src qt6engine)
    endif()
endfunction()

if (BUILD_QT5) 
    build_qtengine("5")
endif()

if (BUILD_QT6) 
    build_qtengine("6")
endif()
