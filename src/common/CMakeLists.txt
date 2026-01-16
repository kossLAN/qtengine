set(QTENGINE_COMMON_SOURCES
  common.cpp
  config/configmanager.cpp
)

add_library(${QTENGINE_NAME}_common SHARED ${QTENGINE_COMMON_SOURCES})
add_library(${QTENGINE_NAME}::common ALIAS ${QTENGINE_NAME}_common)

target_include_directories(${QTENGINE_NAME}_common
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${QTENGINE_NAME}_common
  PUBLIC
    ${QT_NAMESPACE}::Core
    ${QT_NAMESPACE}::Gui
)

if (QT_MAJOR EQUAL 5)
    if (TARGET KF5::ConfigCore)
      target_link_libraries(${QTENGINE_NAME}_common PRIVATE KF5::ConfigCore)
      target_compile_definitions(${QTENGINE_NAME}_common PRIVATE KF_CONFIGCORE_LIB)
    endif()

    if (TARGET KF5::ConfigWidgets)
      target_link_libraries(${QTENGINE_NAME}_common PRIVATE KF5::ConfigWidgets)
      target_compile_definitions(${QTENGINE_NAME}_common PRIVATE KF_COLORSCHEME_LIB)
    endif()
else()
    if (TARGET KF6::ConfigCore)
      target_link_libraries(${QTENGINE_NAME}_common PRIVATE KF6::ConfigCore)
      target_compile_definitions(${QTENGINE_NAME}_common PRIVATE KF_CONFIGCORE_LIB)
    endif()

    if (TARGET KF6::ColorScheme)
      target_link_libraries(${QTENGINE_NAME}_common PRIVATE KF6::ColorScheme)
      target_compile_definitions(${QTENGINE_NAME}_common PRIVATE KF_COLORSCHEME_LIB)
    endif()
endif()

install(TARGETS ${QTENGINE_NAME}_common DESTINATION ${CMAKE_INSTALL_LIBDIR})
