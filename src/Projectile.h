#pragma once
#include "Types.h"

enum class ProjectileType { Bullet, Grenade };

struct Projectile {
    ProjectileType type = ProjectileType::Bullet;
    Vec3 position;
    Vec3 velocity;
    float spin = 0.0f;      // visual rotation angle, degrees
    bool active = false;
};

// radius used for collision against the target/walls
constexpr float PROJECTILE_RADIUS = 1.5f;

void projectile_fire(Projectile& p, ProjectileType type, Vec3 startPos, float yawDegrees);
void projectile_update(Projectile& p, float deltaTime);
void projectile_draw(const Projectile& p);
bool projectile_hit_wall(const Projectile& p);