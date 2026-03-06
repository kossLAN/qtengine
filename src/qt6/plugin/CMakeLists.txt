set(QTENGINE_PLUGIN_SOURCES
	main.cpp
	platformtheme.cpp
)

add_library(Qt6EnginePlugin MODULE ${QTENGINE_PLUGIN_SOURCES})
set_target_properties(Qt6EnginePlugin PROPERTIES OUTPUT_NAME qt6engine-plugin)

target_compile_definitions(Qt6EnginePlugin
    PRIVATE
        $<$<TARGET_EXISTS:Qt6::QuickControls2>:QT_QUICKCONTROLS2_LIB>
)

target_link_libraries(Qt6EnginePlugin
	PUBLIC 
        Qt6::Core 
        Qt6::Gui
	PRIVATE 
        Qt6::GuiPrivate
        Qt6::Widgets
        $<TARGET_NAME_IF_EXISTS:Qt6::QuickControls2>
        KF6::IconThemes
        Qt6Engine::Common
)

install(TARGETS Qt6EnginePlugin DESTINATION "${PLUGINDIR}/platformthemes")
