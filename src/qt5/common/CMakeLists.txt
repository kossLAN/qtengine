set(QTENGINE_COMMON_SOURCES
  common.cpp
  config/configmanager.cpp
  dbus/configwatcher.cpp
)

add_library(Qt5EngineCommon SHARED ${QTENGINE_COMMON_SOURCES})
add_library(Qt5Engine::Common ALIAS Qt5EngineCommon)
set_target_properties(Qt5EngineCommon PROPERTIES OUTPUT_NAME qt5engine-common)

target_include_directories(Qt5EngineCommon
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Qt5EngineCommon PUBLIC Qt5::Core Qt5::DBus)

target_link_libraries(Qt5EngineCommon PRIVATE
  KF5::ConfigCore
  KF5::ConfigWidgets
)

install(TARGETS Qt5EngineCommon DESTINATION ${QT5_INSTALL_LIBDIR})
