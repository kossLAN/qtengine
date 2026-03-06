set(QTENGINE_STYLE_SOURCES
	plugin.cpp
	proxystyle.cpp
)

add_library(Qt5EngineStyle MODULE ${QTENGINE_STYLE_SOURCES})
add_library(Qt5Engine::Style ALIAS Qt5EngineStyle)
set_target_properties(Qt5EngineStyle PROPERTIES
    OUTPUT_NAME qt5engine-style
    INSTALL_RPATH "${QT5_INSTALL_LIBDIR}"
)

target_link_libraries(Qt5EngineStyle
	PRIVATE
		Qt5::Widgets
		Qt5Engine::Common
)

install(TARGETS Qt5EngineStyle DESTINATION "${PLUGINDIR}/styles")
