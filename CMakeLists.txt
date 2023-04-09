cmake_minimum_required(VERSION 3.25)
project(CG_Engine)

# Include directories for GLEW and SFML
include_directories(${CMAKE_SOURCE_DIR}/src/libs/glew-2.1.0/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/SFML-2.5.1/include)

# Include directories for Math, glm and test
include_directories(${CMAKE_SOURCE_DIR}/src/libs/math)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/test)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/glm)

# Link directories for GLEW and SFML
link_directories(${CMAKE_SOURCE_DIR}/src/libs/glew-2.1.0/lib/Release/x64)
link_directories(${CMAKE_SOURCE_DIR}/src/libs/SFML-2.5.1/lib)

# Add executable
add_executable(CG_Engine test.cpp)

# Link GLEW and SFML libraries
target_link_libraries(CG_Engine opengl32 glew32 sfml-graphics sfml-system sfml-window)
