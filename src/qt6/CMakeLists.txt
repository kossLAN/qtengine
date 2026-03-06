if(DEFINED QT6_PLUGINDIR AND NOT QT6_PLUGINDIR STREQUAL "")
    set(PLUGINDIR "${QT6_PLUGINDIR}")
else()
    set(PLUGINDIR "lib/qt6/plugins")
endif()

find_package(Qt6 ${QT6_MIN_VERSION} COMPONENTS Core Gui Widgets DBus REQUIRED)
find_package(Qt6QuickControls2 ${QT6_MIN_VERSION})

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    Config ColorScheme IconThemes
)

if(Qt6_VERSION VERSION_GREATER_EQUAL "6.9.0")
    set(QT_NO_PRIVATE_MODULE_WARNING ON)
    find_package(Qt6 ${QT6_MIN_VERSION} COMPONENTS GuiPrivate REQUIRED)
endif()

add_subdirectory(common)
add_subdirectory(style)
add_subdirectory(plugin)
