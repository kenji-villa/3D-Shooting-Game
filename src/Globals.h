// Shared game state, defined in main.cpp, used by Input.cpp.
#pragma once
#include "Camera.h"
#include "Target.h"
#include "Projectile.h"
#include "GameState.h"

extern Camera g_camera;
extern Target g_target;
extern Projectile g_projectile;
extern GameState g_gameState;
extern int g_windowWidth;
extern int g_windowHeight;