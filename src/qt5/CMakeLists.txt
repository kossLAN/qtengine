if(DEFINED QT5_PLUGINDIR AND NOT QT5_PLUGINDIR STREQUAL "")
    set(PLUGINDIR "${QT5_PLUGINDIR}")
else()
    set(PLUGINDIR "lib/qt5/plugins")
endif()

if(DEFINED QT5_LIBDIR AND NOT QT5_LIBDIR STREQUAL "")
    set(QT5_INSTALL_LIBDIR "${QT5_LIBDIR}")
else()
    set(QT5_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
endif()

find_package(Qt5 ${QT5_MIN_VERSION} COMPONENTS Core Gui Widgets DBus REQUIRED)
find_package(Qt5QuickControls2 ${QT5_MIN_VERSION})
find_package(Qt5ThemeSupport ${QT5_MIN_VERSION} CONFIG REQUIRED Private)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config ConfigWidgets IconThemes
)

add_subdirectory(common)
add_subdirectory(style)
add_subdirectory(plugin)
