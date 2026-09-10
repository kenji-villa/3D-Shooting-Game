// src/Audio.h
#pragma once

// Thin wrapper around miniaudio's high-level engine API.
bool audio_init();
void audio_shutdown();
void audio_play_sound(const char* path); // fire-and-forget playback