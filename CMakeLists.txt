cmake_minimum_required(VERSION 3.2)
project(Erics3DEngine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(LIBS ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})

set(SOURCE_FILES
    src/main.cpp
    src/Erics3DEngine/Engine/Texture.cpp
    src/Erics3DEngine/Engine/Shader.cpp
    src/Erics3DEngine/Engine/Program.cpp
    src/Erics3DEngine/Engine/Application.cpp
    src/Erics3DEngine/Engine/Geometry/Shape.cpp
    src/Erics3DEngine/Engine/Geometry/Cube.cpp
    src/Erics3DEngine/Engine/Geometry/Sphere.cpp
    src/Erics3DEngine/Engine/Geometry/Plane.cpp
    src/Erics3DEngine/Engine/Geometry/TruncatedCone.cpp
    src/Scenes/RainbowTriangleScene.cpp
    src/Scenes/TextureSampleScene.cpp
)
add_executable(Erics3DEngine ${SOURCE_FILES})

link_libraries(${GLFW_LIBRARY_DIRS})
target_link_libraries(Erics3DEngine ${LIBS})