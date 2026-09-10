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
    src/Texture.cpp
    src/Audio.cpp
    src/stb_image.h
    src/miniaudio.h
)

target_include_directories(ShootingGame PRIVATE
    "${FREEGLUT_ROOT}/include"
    "${CMAKE_SOURCE_DIR}/third_party"
)

target_link_directories(ShootingGame PRIVATE
    "${FREEGLUT_ROOT}/lib"
)

target_link_libraries(ShootingGame PRIVATE
    freeglut
    OpenGL::GL
    OpenGL::GLU
)

if(WIN32)
    # miniaudio's Windows backends (WASAPI/WinMM) need these system libs.
    target_link_libraries(ShootingGame PRIVATE winmm ole32)
elseif(UNIX)
    # miniaudio on Linux dlopen()s ALSA/PulseAudio at runtime, but still
    # needs these linked in for the dlopen/threading machinery itself.
    target_link_libraries(ShootingGame PRIVATE pthread m dl)
endif()

add_custom_command(TARGET ShootingGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FREEGLUT_ROOT}/bin/libfreeglut.dll"
        "$<TARGET_FILE_DIR:ShootingGame>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:ShootingGame>/assets"
)
