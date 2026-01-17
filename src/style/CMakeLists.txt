set(QTENGINE_STYLE_SOURCES
	plugin.cpp
	proxystyle.cpp
)

add_library(${QTENGINE_NAME}_style MODULE ${QTENGINE_STYLE_SOURCES})
add_library(${QTENGINE_NAME}::style ALIAS ${QTENGINE_NAME}_style)

target_link_libraries(${QTENGINE_NAME}_style
	PRIVATE
		${QT_NAMESPACE}::Widgets
		${QTENGINE_NAME}::common
)

install(TARGETS ${QTENGINE_NAME}_style DESTINATION "${PLUGINDIR}/styles")
