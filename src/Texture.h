// src/Texture.h
#pragma once

// Loads an image file (PNG/JPG/etc via stb_image) and uploads it as a
// wrapped, mipmapped OpenGL 2D texture. Returns the GL texture id, or 0
// if the file couldn't be loaded (caller should fall back to a flat color).
unsigned int texture_load(const char* path);