cmake_minimum_required(VERSION 3.27.5)

get_filename_component(current_dir "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "" current_dir "${current_dir}")
string(REPLACE "-" "_" current_dir "${current_dir}")

set(CMAKE_VERBOSE_MAKEFILE ON)
message(STATUS "CMake start \n\t directory name: ${current_dir}")

project("${current_dir}" VERSION 0.1 LANGUAGES CXX)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Core Quick Widgets Xml Core5Compat OpenGLWidgets Gui Svg)
find_package(Qt${QT_VERSION_MAJOR} 6.6 REQUIRED COMPONENTS Widgets LinguistTools Core Quick Widgets Xml Core5Compat OpenGLWidgets Gui Svg)

add_subdirectory("../../dynamic_effect/Effect" Effect.out)

file(GLOB_RECURSE ${PROJECT_NAME_UPPER}_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h")
file(GLOB_RECURSE ${PROJECT_NAME_UPPER}_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*")
file(GLOB ${PROJECT_NAME_UPPER}_UI "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")
# add_custom_target(ui SOURCES ${${PROJECT_NAME_UPPER}_UI})
file(GLOB ${PROJECT_NAME_UPPER}_QRC "${CMAKE_CURRENT_SOURCE_DIR}/qrc/*.qrc")
# add_custom_target(qrc SOURCES ${${PROJECT_NAME_UPPER}_QRC})
file(GLOB ${PROJECT_NAME_UPPER}_TS "${CMAKE_CURRENT_SOURCE_DIR}/ts/*.ts")
# add_custom_target(ts SOURCES ${${PROJECT_NAME_UPPER}_TS})

source_group("SRC" FILES ${${PROJECT_NAME_UPPER}_SRC})
source_group("UI" FILES ${${PROJECT_NAME_UPPER}_UI})
source_group("QRC" FILES ${${PROJECT_NAME_UPPER}_QRC})
source_group("TS" FILES ${${PROJECT_NAME_UPPER}_TS})

set(PROJECT_SOURCES
    ${${PROJECT_NAME_UPPER}_HEADER}
    ${${PROJECT_NAME_UPPER}_SRC}
    ${${PROJECT_NAME_UPPER}_UI}
    ${${PROJECT_NAME_UPPER}_QRC}
    ${${PROJECT_NAME_UPPER}_TS}
)

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_SOURCES}
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core5Compat
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt6::Svg
    debug ${QtPropertyBrowser_DIR}/lib/Windows/qtpropertybrowserd.lib
    optimized ${QtPropertyBrowser_DIR}/lib/Windows/qtpropertybrowser.lib
    Effect::Effect
)

if (TARGET ${PROJECT_NAME})
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "${PROJECT_NAME_UPPER}_LIBRARY"
    )
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
    ${QtPropertyBrowser_DIR}/include
    ${Qt6Core_PRIVATE_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_MESSAGELOGCONTEXT)
