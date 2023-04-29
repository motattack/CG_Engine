cmake_minimum_required(VERSION 3.25)
project(CG_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Copy the "res" folder to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -g")

# Include directories for GLEW and SFML
include_directories(${CMAKE_SOURCE_DIR}/src/libs/glew-2.1.0/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/SFML-2.5.1/include)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/assimp-5.2.5/include)

# Include directories for Math, glm and test
include_directories(${CMAKE_SOURCE_DIR}/src/libs/math)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/shader)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/vertex)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/test)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/glm)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/camera)
include_directories(${CMAKE_SOURCE_DIR}/src/libs/model)

include_directories(${CMAKE_SOURCE_DIR}/src/libs/stb)

# Link directories for GLEW and SFML
link_directories(${CMAKE_SOURCE_DIR}/src/libs/glew-2.1.0/lib/Release/x64)
link_directories(${CMAKE_SOURCE_DIR}/src/libs/SFML-2.5.1/lib)
link_directories(${CMAKE_SOURCE_DIR}/src/libs/assimp-5.2.5/lib)

include_directories(${CMAKE_SOURCE_DIR}/src/libs/imgui-sfml-2.6)
file(GLOB IMGUI_SFML_SRCS ${CMAKE_SOURCE_DIR}/src/libs/imgui-sfml-2.6/*.cpp)

# Add executable
add_executable(CG_Engine main.cpp ${IMGUI_SFML_SRCS})

# Link GLEW and SFML libraries
target_link_libraries(CG_Engine libassimp-5 opengl32 glew32 sfml-graphics sfml-system sfml-window)