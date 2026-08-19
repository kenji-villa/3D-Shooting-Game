// src/main.cpp
#include <GL/freeglut.h>
#include "Globals.h"
#include "Renderer.h"
#include "Input.h"
#include "Room.h"

// ---- Global game state (declared extern in Globals.h) ----
Camera g_camera;
Target g_target;
Projectile g_projectile;
GameState g_gameState;
int g_windowWidth = 1080;
int g_windowHeight = 720;

static int g_lastTimeMs = 0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera_apply(g_camera);
    renderer_draw_scene(g_target, g_projectile);

    glutSwapBuffers();
}

void idle() {
    int now = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (now - g_lastTimeMs) / 1000.0f;
    g_lastTimeMs = now;

    gamestate_update(g_gameState, g_projectile, g_target, deltaTime);

    glutPostRedisplay();
}

void reshape(int w, int h) {
    g_windowWidth = w;
    g_windowHeight = (h == 0) ? 1 : h;

    glViewport(0, 0, g_windowWidth, g_windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, static_cast<double>(g_windowWidth) / g_windowHeight, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(g_windowWidth, g_windowHeight);
    glutCreateWindow("Aim and Shoot");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    renderer_setup_lights();

    camera_init(g_camera, Vec3(0.0f, 15.0f, ROOM_NEAR_Z), Vec3(0.0f, 1.0f, 0.0f), 100.0f);
    target_init(g_target, Vec3(0.0f, 15.0f, -30.0f), 5.0f);
    gamestate_reset(g_gameState, g_target);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(input_passive_motion);
    glutKeyboardUpFunc(input_key_up);

    glutSetCursor(GLUT_CURSOR_NONE);
    g_lastTimeMs = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();
    return 0;
}