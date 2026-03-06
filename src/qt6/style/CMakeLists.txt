set(QTENGINE_STYLE_SOURCES
	plugin.cpp
	proxystyle.cpp
)

add_library(Qt6EngineStyle MODULE ${QTENGINE_STYLE_SOURCES})
add_library(Qt6Engine::Style ALIAS Qt6EngineStyle)
set_target_properties(Qt6EngineStyle PROPERTIES OUTPUT_NAME qt6engine-style)

target_link_libraries(Qt6EngineStyle
	PRIVATE
		Qt6::Widgets
		Qt6Engine::Common
)

install(TARGETS Qt6EngineStyle DESTINATION "${PLUGINDIR}/styles")
