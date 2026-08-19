// src/Target.cpp
#include "Target.h"
#include <GL/freeglut.h>

void target_init(Target& t, Vec3 position, float radius) {
    t.position = position;
    t.radius = radius;
    t.alive = true;
}

void target_draw(const Target& t) {
    if (!t.alive) return;

    glPushMatrix();
    glTranslatef(t.position.x, t.position.y, t.position.z);

    // Ring-style target: a flat disk facing the player, red/white bands.
    GLUquadric* quad = gluNewQuadric();

    glColor3f(0.9f, 0.1f, 0.1f);
    gluDisk(quad, 0.0, t.radius, 32, 1);

    glColor3f(0.95f, 0.95f, 0.95f);
    gluDisk(quad, t.radius * 0.6f, t.radius * 0.75f, 32, 1);

    gluDeleteQuadric(quad);
    glPopMatrix();
}

bool target_check_hit(const Target& t, const Vec3& projectilePos, float projectileRadius) {
    if (!t.alive) return false;
    float dist = (projectilePos - t.position).length();
    return dist <= (t.radius + projectileRadius);
}