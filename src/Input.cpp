// src/Input.cpp
#include "Input.h"
#include "Globals.h"
#include <GL/freeglut.h>
#include <cstdlib>

namespace {
    void fire_current_weapon() {
        if (g_gameState.mode != GameMode::Aiming) return;

        Vec3 startPos = g_camera.eye + camera_aim_direction(g_camera) * 5.0f;
        projectile_fire(g_projectile, g_gameState.currentWeapon, startPos, g_camera.yaw);
        g_gameState.mode = GameMode::Firing;
    }
}

void input_passive_motion(int x, int y) {
    (void)y;
    // Lock aiming once a shot is in the air, same as the reference project.
    if (g_gameState.mode == GameMode::Aiming) {
        camera_handle_mouse(g_camera, x, g_windowWidth);
    }
}

void input_mouse(int button, int state, int x, int y) {
    (void)x; (void)y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        fire_current_weapon();
        glutPostRedisplay();
    }
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

        case 'n': // new round
            gamestate_reset(g_gameState, g_target);
            g_projectile.active = false;
            break;
    }

    glutPostRedisplay();
}