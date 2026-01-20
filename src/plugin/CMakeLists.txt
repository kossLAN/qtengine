set(QTENGINE_PLUGIN_SOURCES
	main.cpp
	platformtheme.cpp
)

add_library(${QTENGINE_NAME}_plugin MODULE ${QTENGINE_PLUGIN_SOURCES})

target_compile_definitions(${QTENGINE_NAME}_plugin
    PRIVATE
        $<$<TARGET_EXISTS:${QT_NAMESPACE}::QuickControls2>:QT_QUICKCONTROLS2_LIB>
)

target_link_libraries(${QTENGINE_NAME}_plugin
	PUBLIC 
        ${QT_NAMESPACE}::Core 
        ${QT_NAMESPACE}::Gui
	PRIVATE 
        ${QT_NAMESPACE}::GuiPrivate
        ${QT_NAMESPACE}::Widgets
        $<TARGET_NAME_IF_EXISTS:${QT_NAMESPACE}::QuickControls2>
        ${KF_NAMESPACE}::IconThemes
        ${QTENGINE_NAME}::common
)

if (QT_MAJOR EQUAL 5)
    target_link_libraries(${QTENGINE_NAME}_plugin PRIVATE 
        ${QT_NAMESPACE}::ThemeSupport 
        ${QT_NAMESPACE}::ThemeSupportPrivate)
endif()

install(TARGETS ${QTENGINE_NAME}_plugin DESTINATION "${PLUGINDIR}/platformthemes")
