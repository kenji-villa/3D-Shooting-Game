// src/Input.cpp
#include "Input.h"
#include "Globals.h"
#include "Audio.h"
#include <GL/freeglut.h>
#include <cstdlib>

namespace {
    void fire_current_weapon() {
        if (g_gameState.mode != GameMode::Aiming) return;

        Vec3 aimDir = camera_aim_direction(g_camera);
        Vec3 startPos = g_camera.eye + aimDir * 5.0f;
        projectile_fire(g_projectile, g_gameState.currentWeapon, startPos, aimDir);
        g_gameState.mode = GameMode::Firing;

        audio_play_sound("assets/cz_trimmed.wav");
    }

    void play_again() {
        gamestate_restart(g_gameState, g_target);
        g_projectile.active = false;
    }
}

void input_passive_motion(int x, int y) {
    // Lock aiming once a shot is in the air, same as the reference project.
    if (g_gameState.mode == GameMode::Aiming) {
        camera_handle_mouse(g_camera, x, y, g_windowWidth, g_windowHeight);
    }
}

void input_mouse(int button, int state, int x, int y) {
    (void)x; (void)y;

    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    if (g_gameState.mode == GameMode::GameOver) {
        play_again(); // clicking anywhere on the Game Over screen restarts
    } else {
        fire_current_weapon();
    }
    glutPostRedisplay();
}

void input_key_up(unsigned char key, int x, int y) {
    (void)x; (void)y;

    switch (key) {
        case 27: // esc
            exit(0);
            break;

        case ' ': // fire (kept alongside left-click)
            fire_current_weapon();
            break;

        case '1': // bullet
            g_gameState.currentWeapon = ProjectileType::Bullet;
            break;

        case '2': // grenade
            g_gameState.currentWeapon = ProjectileType::Grenade;
            break;

        case 'n': // new round (soft reset), or Play Again if Game Over is showing
            if (g_gameState.mode == GameMode::GameOver) {
                play_again();
            } else {
                gamestate_reset(g_gameState, g_target);
                g_projectile.active = false;
            }
            break;
    }

    glutPostRedisplay();
}