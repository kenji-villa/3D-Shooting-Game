// src/Hud.h
#pragma once
#include "GameState.h"

// All HUD drawing happens in a 2D orthographic overlay pass, drawn after
// the 3D scene each frame. Keeping it separate from Renderer.cpp so the
// 3D camera/lighting state and the 2D screen-space state never get mixed.
void hud_draw(const GameState& gs, int windowWidth, int windowHeight);