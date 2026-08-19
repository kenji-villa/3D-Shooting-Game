#pragma once
#include "Projectile.h"
#include "Target.h"

enum class GameMode { Aiming, Firing, HitStop };

struct GameState {
    GameMode mode = GameMode::Aiming;
    ProjectileType currentWeapon = ProjectileType::Bullet;
    int score = 0;
    float hitStopTimer = 0.0f;
};

void gamestate_reset(GameState& gs, Target& target);
void gamestate_switch_weapon(GameState& gs);
void gamestate_update(GameState& gs, Projectile& proj, Target& target, float deltaTime);