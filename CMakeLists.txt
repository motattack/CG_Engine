cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME CG_Engine)
project(${PROJECT_NAME})

add_subdirectory(CG_Engine_Core)
add_subdirectory(CG_Engine_Editor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CG_Engine_Editor)