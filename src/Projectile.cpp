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
}

void projectile_fire(Projectile& p, ProjectileType type, Vec3 startPos, float yawDegrees) {
    p.type = type;
    p.position = startPos;
    p.spin = 0.0f;
    p.active = true;

    float rad = yawDegrees * TO_RAD;
    Vec3 dir(std::sin(rad), 0.0f, -std::cos(rad)); // matches Camera's aim direction

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
    glRotatef(p.spin, 0.0f, 0.0f, 1.0f);

    GLUquadric* quad = gluNewQuadric();

    if (p.type == ProjectileType::Bullet) {
        glColor3f(0.4f, 0.4f, 0.4f);
        gluCylinder(quad, PROJECTILE_RADIUS, PROJECTILE_RADIUS * 0.4f, 4.0f, 16, 4);
    } else {
        glColor3f(0.1f, 0.4f, 0.15f);
        glutSolidSphere(PROJECTILE_RADIUS, 16, 16);
    }

    gluDeleteQuadric(quad);
    glPopMatrix();
}