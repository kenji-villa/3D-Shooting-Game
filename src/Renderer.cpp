#include "Renderer.h"
#include "Room.h"
#include <GL/freeglut.h>

void renderer_setup_lights() {
    GLfloat ambient[]  = { 0.35f, 0.35f, 0.35f, 1.0f };
    GLfloat diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0.0f, 70.0f, 60.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void renderer_draw_scene(const Target& target, const Projectile& proj) {
    room_draw();
    target_draw(target);
    projectile_draw(proj);
}