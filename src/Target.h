// src/Target.h
#pragma once
#include "Types.h"

struct Target {
    Vec3 position;
    float radius = 5.0f;
    bool alive = true;
};

void target_init(Target& t, Vec3 position, float radius);
void target_randomize_position(Target& t);
void target_draw(const Target& t);
// Returns points scored: 0 = miss, 1 (outer white) through 5 (gold bullseye).
int target_check_hit(const Target& t, const Vec3& projectilePos, float projectileRadius);