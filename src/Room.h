// src/Room.h
#pragma once

// Static room bounds — shared with Projectile.cpp's wall-collision check.
constexpr float ROOM_HALF_WIDTH = 60.0f;   // +/- X
constexpr float ROOM_FLOOR_Y = 0.0f;
constexpr float ROOM_CEILING_Y = 80.0f;
constexpr float ROOM_NEAR_Z = 150.0f;      // player stands here
constexpr float ROOM_FAR_Z = -60.0f;       // back wall

void room_draw(unsigned int floorTextureId, unsigned int wallTextureId);