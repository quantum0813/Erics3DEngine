cmake_minimum_required(VERSION 3.2)
project(Erics3DEngine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(LeapMotion REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(LIBS
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    assimp
    Leap
    Bullet3Collision
    Bullet3Common
    Bullet3Dynamics
    Bullet3Geometry
)

set(SOURCE_FILES
    src/main.cpp
    src/Erics3DEngine/Engine/Texture.cpp
    src/Erics3DEngine/Engine/Shader.cpp
    src/Erics3DEngine/Engine/Program.cpp
    src/Erics3DEngine/Engine/Application.cpp
    src/Erics3DEngine/Engine/Input.cpp
    src/Erics3DEngine/Engine/Camera.cpp
    src/Erics3DEngine/Engine/Geometry/Shape.cpp
    src/Erics3DEngine/Engine/Geometry/Cube.cpp
    src/Erics3DEngine/Engine/Geometry/Sphere.cpp
    src/Erics3DEngine/Engine/Geometry/Plane.cpp
    src/Erics3DEngine/Engine/Geometry/TruncatedCone.cpp
    src/Erics3DEngine/Engine/Model/Mesh.cpp
    src/Erics3DEngine/Engine/Model/Model.cpp
    src/Erics3DEngine/Engine/TextOverlay.cpp
    src/Scenes/RainbowTriangleScene.cpp
    src/Scenes/TextureSampleScene.cpp
    src/Scenes/LeapListener.cpp
    src/Scenes/LeapMotionScene.cpp
    src/Scenes/PhysicsScene.cpp
    src/Scenes/FlockingScene.cpp
    src/Scenes/GrassScene.cpp
    src/Scenes/CubicBezierScene.cpp
    src/Scenes/StarfieldScene.cpp
    src/Scenes/JuliaScene.cpp
)
add_executable(Erics3DEngine ${SOURCE_FILES})

link_libraries(${GLFW_LIBRARY_DIRS})
target_link_libraries(Erics3DEngine ${LIBS})