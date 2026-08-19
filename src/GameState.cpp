#include "GameState.h"

namespace {
    constexpr float HIT_STOP_DURATION = 1.2f;
    constexpr float MISS_STOP_DURATION = 0.6f;
}

void gamestate_reset(GameState& gs, Target& target) {
    gs.mode = GameMode::Aiming;
    gs.hitStopTimer = 0.0f;
    target.alive = true;
    // currentWeapon and score are intentionally left as-is on reset;
    // call target_init() separately if you want to reposition the target too.
}

void gamestate_switch_weapon(GameState& gs) {
    if (gs.mode != GameMode::Aiming) return; // no switching mid-flight

    gs.currentWeapon = (gs.currentWeapon == ProjectileType::Bullet)
        ? ProjectileType::Grenade
        : ProjectileType::Bullet;
}

void gamestate_update(GameState& gs, Projectile& proj, Target& target, float deltaTime) {
    if (gs.mode == GameMode::Firing) {
        projectile_update(proj, deltaTime);

        if (target_check_hit(target, proj.position, PROJECTILE_RADIUS)) {
            gs.score++;
            target.alive = false;
            proj.active = false;
            gs.mode = GameMode::HitStop;
            gs.hitStopTimer = HIT_STOP_DURATION;
        } else if (projectile_hit_wall(proj)) {
            proj.active = false;
            gs.mode = GameMode::HitStop;
            gs.hitStopTimer = MISS_STOP_DURATION;
        }
    } else if (gs.mode == GameMode::HitStop) {
        gs.hitStopTimer -= deltaTime;
        if (gs.hitStopTimer <= 0.0f) {
            if (!target.alive) {
                target_randomize_position(target); // move it before it reappears
                target.alive = true;
            }
            gs.mode = GameMode::Aiming;
        }
    }
}