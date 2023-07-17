cmake_minimum_required(VERSION 3.25)

set(ENGINE_PROJECT_NAME CG_Engine)
project(${ENGINE_PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
add_link_options(-static-libgcc -static-libstdc++)

# Set the path to the GLEW library and include directory
set(GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/glew-2.1.0/include)
set(GLEW_LIB_DIRS ${CMAKE_SOURCE_DIR}/external/glew-2.1.0/lib/Release/x64)

# Add the GLEW library
add_library(glew STATIC IMPORTED)
set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${GLEW_LIB_DIRS}/glew32.lib)

# Set the path to the assimp library and include directory
set(ASSIMP_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/assimp-5.2.5/include)
set(ASSIMP_LIB_DIRS ${CMAKE_SOURCE_DIR}/external/assimp-5.2.5/lib)

add_library(assimp STATIC IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${ASSIMP_LIB_DIRS}/libassimp-5.dll)

# Set the path to the SFML library and include directory
set(SFML_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/SFML-2.5.1/include)
set(SFML_LIB_DIRS ${CMAKE_SOURCE_DIR}/external/SFML-2.5.1/lib)

# Add the SFML libraries
add_library(sfml-window STATIC IMPORTED)
set_target_properties(sfml-window PROPERTIES IMPORTED_LOCATION ${SFML_LIB_DIRS}/libsfml-window.a)
add_library(sfml-system STATIC IMPORTED)
set_target_properties(sfml-system PROPERTIES IMPORTED_LOCATION ${SFML_LIB_DIRS}/libsfml-system.a)
add_library(sfml-graphics STATIC IMPORTED)
set_target_properties(sfml-graphics PROPERTIES IMPORTED_LOCATION ${SFML_LIB_DIRS}/libsfml-graphics.a)
add_library(sfml-audio STATIC IMPORTED)
set_target_properties(sfml-audio PROPERTIES IMPORTED_LOCATION ${SFML_LIB_DIRS}/libsfml-audio.a)


# Set the path to the IMGUI_SFML directory and source files
set(IMGUI_SFML_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/imgui-sfml-2.6)
file(GLOB IMGUI_SFML_SOURCE_FILES ${CMAKE_SOURCE_DIR}/external/imgui-sfml-2.6/*.cpp)

# Set the path to the STB directory
set(STB_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/stb)

# Set the path to the CATCH directory
set(CATCH_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/test)

# Set the path to the GLM directory
set(GLM_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external/glm)

add_executable(${ENGINE_PROJECT_NAME}
        main.cpp
        includes/CG_Engine/engine.cpp
        ${IMGUI_SFML_SOURCE_FILES}
        )

# Link GLEW and SFML to the engine core library
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glew opengl32 sfml-graphics sfml-system sfml-window assimp)

# Add the engine core include directories
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)

# Add the GLEW, SFML, and IMGUI_SFML include directories to the engine core include directories
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC ${CATCH_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SFML_INCLUDE_DIRS} ${IMGUI_SFML_INCLUDE_DIRS} ${STB_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

set_target_properties(${ENGINE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)