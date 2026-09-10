// src/Projectile.cpp
#include "Projectile.h"
#include "Room.h"
#include <GL/freeglut.h>
#include <cmath>

namespace {
    constexpr float BULLET_SPEED   = 90.0f;   // units/sec, flat trajectory
    constexpr float GRENADE_SPEED_XZ = 45.0f; // units/sec, horizontal component
    constexpr float GRENADE_SPEED_Y  = 25.0f; // units/sec, initial upward kick
    constexpr float GRAVITY = 45.0f;          // units/sec^2

    void update_bullet(Projectile& p, float dt) {
        p.position += p.velocity * dt;
        p.spin += 720.0f * dt; // spins fast around its travel axis, purely visual
    }

    void update_grenade(Projectile& p, float dt) {
        p.velocity.y -= GRAVITY * dt;
        p.position += p.velocity * dt;
        p.spin += 180.0f * dt; // tumbles slower than a bullet
    }

    // Rotates the current modelview so local +Z (the axis gluCylinder/gluCone
    // extrude along) points along the given direction. Without this, an
    // elongated model always faces world +Z regardless of which way it's
    // actually travelling — from most viewing angles that looks like it's
    // tumbling sideways rather than flying nose-first.
    void apply_direction_rotation(const Vec3& velocity) {
        Vec3 dir = velocity.normalized();
        if (dir.length() < 1e-5f) return; // no meaningful direction to align to

        Vec3 zAxis(0.0f, 0.0f, 1.0f);
        float d = zAxis.dot(dir);
        Vec3 axis = zAxis.cross(dir);
        float axisLen = axis.length();

        if (d < -0.9999f) {
            // Pointing exactly backward — 180 degrees about any perpendicular axis works.
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        } else if (axisLen > 1e-5f) {
            float clampedD = d < -1.0f ? -1.0f : (d > 1.0f ? 1.0f : d);
            float angleDeg = std::acos(clampedD) * (180.0f / PI);
            glRotatef(angleDeg, axis.x, axis.y, axis.z);
        }
        // else dir is already ~= +Z, no rotation needed
    }
}

void projectile_fire(Projectile& p, ProjectileType type, Vec3 startPos, Vec3 direction) {
    p.type = type;
    p.position = startPos;
    p.spin = 0.0f;
    p.active = true;

    Vec3 dir = direction.normalized();

    if (type == ProjectileType::Bullet) {
        p.velocity = dir * BULLET_SPEED;
    } else {
        p.velocity = dir * GRENADE_SPEED_XZ + Vec3(0.0f, GRENADE_SPEED_Y, 0.0f);
    }
}

void projectile_update(Projectile& p, float dt) {
    if (!p.active) return;

    if (p.type == ProjectileType::Bullet) {
        update_bullet(p, dt);
    } else {
        update_grenade(p, dt);
    }
}

bool projectile_hit_wall(const Projectile& p) {
    if (!p.active) return false;

    return p.position.z <= ROOM_FAR_Z ||
           p.position.z >= ROOM_NEAR_Z + 5.0f ||
           p.position.x <= -ROOM_HALF_WIDTH ||
           p.position.x >=  ROOM_HALF_WIDTH ||
           p.position.y <=  ROOM_FLOOR_Y ||       // grenade lands / bullet dips below floor
           p.position.y >=  ROOM_CEILING_Y;
}

void projectile_draw(const Projectile& p) {
    if (!p.active) return;

    glPushMatrix();
    glTranslatef(p.position.x, p.position.y, p.position.z);

    GLUquadric* quad = gluNewQuadric();

    if (p.type == ProjectileType::Bullet) {
        apply_direction_rotation(p.velocity); // point the model where it's actually flying
        glRotatef(p.spin, 0.0f, 0.0f, 1.0f);   // roll around its own long axis

        constexpr float BODY_LENGTH = 4.0f;
        constexpr float NOSE_LENGTH = 2.5f;

        // Brass casing (blunt rear section)
        glColor3f(0.70f, 0.55f, 0.20f);
        gluCylinder(quad, PROJECTILE_RADIUS, PROJECTILE_RADIUS, BODY_LENGTH, 12, 1);

        // Flat base cap so the rear isn't hollow when seen from behind
        glPushMatrix();
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        gluDisk(quad, 0.0, PROJECTILE_RADIUS, 12, 1);
        glPopMatrix();

        // Copper pointed nose — tapers almost to a point, like a real ogive tip
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, BODY_LENGTH);
        glColor3f(0.80f, 0.47f, 0.25f);
        gluCylinder(quad, PROJECTILE_RADIUS, PROJECTILE_RADIUS * 0.08f, NOSE_LENGTH, 12, 4);
        glPopMatrix();
    } else {
        glColor3f(0.1f, 0.4f, 0.15f);
        glutSolidSphere(PROJECTILE_RADIUS, 16, 16);
    }

    gluDeleteQuadric(quad);
    glPopMatrix();
}