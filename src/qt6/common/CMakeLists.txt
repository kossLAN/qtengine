set(QTENGINE_COMMON_SOURCES
  common.cpp
  config/configmanager.cpp
  dbus/configwatcher.cpp
)

add_library(Qt6EngineCommon SHARED ${QTENGINE_COMMON_SOURCES})
add_library(Qt6Engine::Common ALIAS Qt6EngineCommon)
set_target_properties(Qt6EngineCommon PROPERTIES OUTPUT_NAME qt6engine-common)

target_include_directories(Qt6EngineCommon
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Qt6EngineCommon PUBLIC Qt6::Core Qt6::DBus)

target_link_libraries(Qt6EngineCommon PRIVATE
  KF6::ConfigCore
  KF6::ColorScheme
)

install(TARGETS Qt6EngineCommon DESTINATION ${CMAKE_INSTALL_LIBDIR})
