// src/Audio.cpp
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "Audio.h"
#include <cstdio>

namespace {
    ma_engine g_engine;
    bool g_initialized = false;
}

bool audio_init() {
    if (ma_engine_init(NULL, &g_engine) != MA_SUCCESS) {
        std::fprintf(stderr, "audio_init: failed to initialize audio engine\n");
        return false;
    }
    g_initialized = true;
    return true;
}

void audio_shutdown() {
    if (g_initialized) {
        ma_engine_uninit(&g_engine);
        g_initialized = false;
    }
}

void audio_play_sound(const char* path) {
    if (!g_initialized) return;

    // ma_engine_play_sound is fire-and-forget: it manages the sound's
    // lifetime internally, so rapid-fire calls (mashing the fire button)
    // won't leak or need manual cleanup.
    if (ma_engine_play_sound(&g_engine, path, NULL) != MA_SUCCESS) {
        std::fprintf(stderr, "audio_play_sound: failed to play '%s'\n", path);
    }
}