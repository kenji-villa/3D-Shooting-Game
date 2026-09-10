#pragma once
#include "Projectile.h"
#include "Target.h"

enum class GameMode { Aiming, Firing, HitStop, GameOver };

constexpr int WIN_SCORE = 20; // reaching this score ends the round

struct GameState {
    GameMode mode = GameMode::Aiming;
    ProjectileType currentWeapon = ProjectileType::Bullet;
    int score = 0;
    float hitStopTimer = 0.0f;
};

// Soft reset: clears the current shot/target state but keeps score and
// weapon as-is. Used for the mid-round "n" new-round hotkey.
void gamestate_reset(GameState& gs, Target& target);

// Full reset: used to start a fresh round after Game Over ("Play Again").
void gamestate_restart(GameState& gs, Target& target);

void gamestate_switch_weapon(GameState& gs);
void gamestate_update(GameState& gs, Projectile& proj, Target& target, float deltaTime);