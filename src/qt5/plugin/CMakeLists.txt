set(QTENGINE_PLUGIN_SOURCES
	main.cpp
	platformtheme.cpp
)

add_library(Qt5EnginePlugin MODULE ${QTENGINE_PLUGIN_SOURCES})
set_target_properties(Qt5EnginePlugin PROPERTIES
    OUTPUT_NAME qt5engine-plugin
    INSTALL_RPATH "${QT5_INSTALL_LIBDIR}"
)

target_compile_definitions(Qt5EnginePlugin
    PRIVATE
        $<$<TARGET_EXISTS:Qt5::QuickControls2>:QT_QUICKCONTROLS2_LIB>
)

target_link_libraries(Qt5EnginePlugin
	PUBLIC 
        Qt5::Core 
        Qt5::Gui
	PRIVATE 
        Qt5::GuiPrivate
        Qt5::Widgets
        $<TARGET_NAME_IF_EXISTS:Qt5::QuickControls2>
        KF5::IconThemes
        Qt5Engine::Common
)

target_link_libraries(Qt5EnginePlugin PRIVATE 
    Qt5::ThemeSupport 
    Qt5::ThemeSupportPrivate)

install(TARGETS Qt5EnginePlugin DESTINATION "${PLUGINDIR}/platformthemes")
