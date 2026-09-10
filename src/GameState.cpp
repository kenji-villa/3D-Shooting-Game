#include "GameState.h"

namespace {
    constexpr float HIT_STOP_DURATION = 1.2f;
    constexpr float MISS_STOP_DURATION = 0.6f;
}

void gamestate_reset(GameState& gs, Target& target) {
    gs.mode = GameMode::Aiming;
    gs.hitStopTimer = 0.0f;
    target.alive = true;
    // currentWeapon and score are intentionally left as-is on a soft reset;
    // call target_init() separately if you want to reposition the target too.
}

void gamestate_restart(GameState& gs, Target& target) {
    gs.mode = GameMode::Aiming;
    gs.hitStopTimer = 0.0f;
    gs.score = 0;
    target_randomize_position(target);
    target.alive = true;
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

        int points = target_check_hit(target, proj.position, PROJECTILE_RADIUS);
        if (points > 0) {
            gs.score += points;
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
            if (gs.score >= WIN_SCORE) {
                gs.mode = GameMode::GameOver;
            } else {
                if (!target.alive) {
                    target_randomize_position(target); // move it before it reappears
                    target.alive = true;
                }
                gs.mode = GameMode::Aiming;
            }
        }
    }
    // GameMode::GameOver: intentionally idle here — everything freezes until
    // the player restarts via Input.cpp's gamestate_restart() call.
}