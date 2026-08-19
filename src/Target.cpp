#include "Target.h"
#include "Room.h"
#include <GL/freeglut.h>
#include <cstdlib>

namespace {
    // Keep the target off the side walls and somewhere in the back half of
    // the room — never right in the player's face, never clipping a wall.
    constexpr float TARGET_MIN_X = -ROOM_HALF_WIDTH + 15.0f;
    constexpr float TARGET_MAX_X =  ROOM_HALF_WIDTH - 15.0f;
    constexpr float TARGET_MIN_Y = 8.0f;
    constexpr float TARGET_MAX_Y = 35.0f;
    constexpr float TARGET_MIN_Z = ROOM_FAR_Z + 10.0f;
    constexpr float TARGET_MAX_Z = ROOM_FAR_Z + 70.0f;

    float random_range(float lo, float hi) {
        float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        return lo + t * (hi - lo);
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