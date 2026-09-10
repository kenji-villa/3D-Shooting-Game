// src/Target.cpp
#include "Target.h"
#include "Room.h"
#include <GL/freeglut.h>
#include <cstdlib>
#include <cmath>

namespace {
    // Keep the target off the side walls and somewhere in the back half of
    // the room — never right in the player's face, never clipping a wall.
    constexpr float TARGET_MIN_X = -ROOM_HALF_WIDTH + 15.0f;
    constexpr float TARGET_MAX_X =  ROOM_HALF_WIDTH - 15.0f;
    constexpr float TARGET_MIN_Y = 8.0f;
    constexpr float TARGET_MAX_Y = 35.0f;
    constexpr float TARGET_MIN_Z = ROOM_FAR_Z + 10.0f;
    constexpr float TARGET_MAX_Z = ROOM_FAR_Z + 70.0f;

    // The player stands at ROOM_NEAR_Z (see Camera init in main.cpp). Targets
    // spawn anywhere between TARGET_MIN_Z and TARGET_MAX_Z, so distance from
    // the player ranges accordingly — used to scale target size so distant
    // targets, which shrink more under perspective, stay a fair size to hit.
    constexpr float MIN_PLAYER_DIST = ROOM_NEAR_Z - TARGET_MAX_Z; // closest possible spawn
    constexpr float MAX_PLAYER_DIST = ROOM_NEAR_Z - TARGET_MIN_Z; // farthest possible spawn
    constexpr float NEAR_SCALE = 1.0f;
    constexpr float FAR_SCALE = 1.7f;

    float random_range(float lo, float hi) {
        float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        return lo + t * (hi - lo);
    }

    float clamp01(float v) {
        if (v < 0.0f) return 0.0f;
        if (v > 1.0f) return 1.0f;
        return v;
    }

    // The actual on-screen/hit-detection radius, after distance scaling.
    // Both target_draw and target_check_hit go through this so what you see
    // is exactly what you can hit.
    float effective_radius(const Target& t) {
        float dist = ROOM_NEAR_Z - t.position.z;
        float f = clamp01((dist - MIN_PLAYER_DIST) / (MAX_PLAYER_DIST - MIN_PLAYER_DIST));
        float scale = NEAR_SCALE + f * (FAR_SCALE - NEAR_SCALE);
        return t.radius * scale;
    }

    // Draws a full disk of the given radius, nudged toward the camera by
    // zOffset relative to the target's origin. Layering rings back-to-front
    // with increasing zOffset avoids z-fighting (equal-depth quads at the
    // exact same z can fail the depth test unpredictably) and guarantees
    // each inner ring visibly sits in front of the one behind it.
    void draw_disk_layer(GLUquadric* quad, float radius, float zOffset,
                          float r, float g, float b) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, zOffset);
        glColor3f(r, g, b);
        gluDisk(quad, 0.0, radius, 32, 1);
        glPopMatrix();
    }
}

void target_init(Target& t, Vec3 position, float radius) {
    t.position = position;
    t.radius = radius;
    t.alive = true;
}

void target_randomize_position(Target& t) {
    t.position.x = random_range(TARGET_MIN_X, TARGET_MAX_X);
    t.position.y = random_range(TARGET_MIN_Y, TARGET_MAX_Y);
    t.position.z = random_range(TARGET_MIN_Z, TARGET_MAX_Z);
}

void target_draw(const Target& t) {
    if (!t.alive) return;

    GLUquadric* quad = gluNewQuadric();
    float R = effective_radius(t);

    // --- Support post: a wooden post running from the target down to the
    // floor, so it reads as a mounted range target instead of floating.
    glColor3f(0.32f, 0.20f, 0.10f);
    glPushMatrix();
    glTranslatef(t.position.x, t.position.y, t.position.z);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // point the cylinder's extrusion axis straight down
    float postRadius = R * 0.12f;
    gluCylinder(quad, postRadius, postRadius, t.position.y - ROOM_FLOOR_Y, 12, 1);
    glPopMatrix();

    // Small round foot where the post meets the floor, for visual weight.
    glPushMatrix();
    glTranslatef(t.position.x, ROOM_FLOOR_Y + 0.05f, t.position.z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0, postRadius * 3.0f, 12, 1);
    glPopMatrix();

    // --- Target head: wood backing board + concentric bullseye rings,
    // all facing the player (local +Z, same orientation the original used).
    glPushMatrix();
    glTranslatef(t.position.x, t.position.y, t.position.z);

    // Square wood backing board, slightly larger than the rings themselves.
    glColor3f(0.42f, 0.27f, 0.14f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.08f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-R * 1.35f, -R * 1.35f, 0.0f);
        glVertex3f( R * 1.35f, -R * 1.35f, 0.0f);
        glVertex3f( R * 1.35f,  R * 1.35f, 0.0f);
        glVertex3f(-R * 1.35f,  R * 1.35f, 0.0f);
    glEnd();
    glPopMatrix();

    // Bullseye: white, purple, blue, red, yellow — each layer a touch closer
    // to the camera than the last. Radii here must match the score bands in
    // target_check_hit below.
    draw_disk_layer(quad, R * 1.0f,  -0.05f, 0.95f, 0.95f, 0.95f); // white  (1 pt)
    draw_disk_layer(quad, R * 0.8f,  -0.04f, 0.45f, 0.10f, 0.55f); // purple (2 pts)
    draw_disk_layer(quad, R * 0.6f,  -0.03f, 0.10f, 0.30f, 0.75f); // blue   (3 pts)
    draw_disk_layer(quad, R * 0.4f,  -0.02f, 0.80f, 0.10f, 0.10f); // red    (4 pts)
    draw_disk_layer(quad, R * 0.2f,  -0.01f, 1.00f, 0.92f, 0.10f); // yellow (5 pts)

    glPopMatrix();

    gluDeleteQuadric(quad);
}

int target_check_hit(const Target& t, const Vec3& projectilePos, float projectileRadius) {
    if (!t.alive) return 0;

    float R = effective_radius(t);
    Vec3 offset = projectilePos - t.position;
    float dist3D = offset.length();

    if (dist3D > R + projectileRadius) return 0; // miss entirely

    // Which ring was hit depends on how far off-center the shot was *across
    // the target's face* (X/Y), not the full 3D distance to the target's
    // center. Using dist3D here would be skewed by how much leftover depth
    // the projectile still had to travel when the hit was first detected
    // (bullets move in fast discrete steps, not continuously) — a dead-center
    // shot could still show a large 3D distance at the exact frame the hit
    // registers, even though it was perfectly aimed.
    float lateralDist = std::sqrt(offset.x * offset.x + offset.y * offset.y);
    float normalized = lateralDist / R;

    if (normalized <= 0.2f) return 5; // yellow bullseye
    if (normalized <= 0.4f) return 4; // red
    if (normalized <= 0.6f) return 3; // blue
    if (normalized <= 0.8f) return 2; // purple
    return 1;                         // white outer ring
}