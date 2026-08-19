cmake_minimum_required(VERSION 3.16)

project(ShootingGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FREEGLUT_ROOT "C:/freeglut-3.2.1/install")

find_package(OpenGL REQUIRED)

add_executable(ShootingGame
    src/main.cpp
    src/Camera.cpp
    src/Room.cpp
    src/Target.cpp
    src/Projectile.cpp
    src/GameState.cpp
    src/Renderer.cpp
    src/Input.cpp
    src/Hud.cpp
)

target_include_directories(ShootingGame PRIVATE
    "${FREEGLUT_ROOT}/include"
)

target_link_directories(ShootingGame PRIVATE
    "${FREEGLUT_ROOT}/lib"
)

target_link_libraries(ShootingGame PRIVATE
    freeglut
    OpenGL::GL
    OpenGL::GLU
)

add_custom_command(TARGET ShootingGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FREEGLUT_ROOT}/bin/libfreeglut.dll"
        "$<TARGET_FILE_DIR:ShootingGame>"
)