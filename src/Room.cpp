// src/Room.cpp
#include "Room.h"
#include <GL/freeglut.h>

namespace {
    // How many times the floor texture tiles across it — tune to taste
    // depending on the source image's real-world scale.
    constexpr float FLOOR_TEX_REPEAT_X = 8.0f;
    constexpr float FLOOR_TEX_REPEAT_Z = 14.0f;

    // World units per wall-texture tile — used to derive each wall's repeat
    // count from its actual size, so tile scale looks consistent across the
    // back wall (short) and side walls (long) instead of stretching.
    constexpr float WALL_TEX_UNIT_SIZE = 15.0f;

    constexpr float ROOM_WIDTH = ROOM_HALF_WIDTH * 2.0f;
    constexpr float ROOM_DEPTH = ROOM_NEAR_Z - ROOM_FAR_Z;
    constexpr float WALL_TEX_REPEAT_HORIZ_SHORT = ROOM_WIDTH / WALL_TEX_UNIT_SIZE; // back wall
    constexpr float WALL_TEX_REPEAT_HORIZ_LONG  = ROOM_DEPTH / WALL_TEX_UNIT_SIZE; // side walls
    constexpr float WALL_TEX_REPEAT_VERT        = ROOM_CEILING_Y / WALL_TEX_UNIT_SIZE;

    void draw_textured_quad(float uMax, float vMax,
                             float x0, float y0, float z0,
                             float x1, float y1, float z1,
                             float x2, float y2, float z2,
                             float x3, float y3, float z3,
                             float nx, float ny, float nz) {
        glNormal3f(nx, ny, nz);
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(x0, y0, z0);
        glTexCoord2f(uMax, 0.0f);   glVertex3f(x1, y1, z1);
        glTexCoord2f(uMax, vMax);   glVertex3f(x2, y2, z2);
        glTexCoord2f(0.0f, vMax);   glVertex3f(x3, y3, z3);
    }

    void draw_flat_quad(float x0, float y0, float z0,
                         float x1, float y1, float z1,
                         float x2, float y2, float z2,
                         float x3, float y3, float z3,
                         float nx, float ny, float nz) {
        glNormal3f(nx, ny, nz);
        glVertex3f(x0, y0, z0);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
    }
}

void room_draw(unsigned int floorTextureId, unsigned int wallTextureId) {
    // Floor
    if (floorTextureId != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, floorTextureId);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
            draw_textured_quad(FLOOR_TEX_REPEAT_X, FLOOR_TEX_REPEAT_Z,
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_NEAR_Z,
                 ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_NEAR_Z,
                 ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_FAR_Z,
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_FAR_Z,
                0, 1, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    } else {
        glColor3f(0.55f, 0.55f, 0.55f);
        glBegin(GL_QUADS);
            draw_flat_quad(
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_NEAR_Z,
                 ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_NEAR_Z,
                 ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_FAR_Z,
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_FAR_Z,
                0, 1, 0);
        glEnd();
    }

    // Walls
    if (wallTextureId != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, wallTextureId);
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
            // Back wall
            draw_textured_quad(WALL_TEX_REPEAT_HORIZ_SHORT, WALL_TEX_REPEAT_VERT,
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z,
                 ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z,
                 ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z,
                -ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z,
                0, 0, 1);

            // Left wall
            draw_textured_quad(WALL_TEX_REPEAT_HORIZ_LONG, WALL_TEX_REPEAT_VERT,
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_NEAR_Z,
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z,
                -ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z,
                -ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_NEAR_Z,
                1, 0, 0);

            // Right wall
            draw_textured_quad(WALL_TEX_REPEAT_HORIZ_LONG, WALL_TEX_REPEAT_VERT,
                ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z,
                ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_NEAR_Z,
                ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_NEAR_Z,
                ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z,
                -1, 0, 0);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    } else {
        glBegin(GL_QUADS);
            glColor3f(0.65f, 0.65f, 0.7f);
            draw_flat_quad(
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z,
                 ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z,
                 ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z,
                -ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z,
                0, 0, 1);

            glColor3f(0.6f, 0.6f, 0.65f);
            draw_flat_quad(
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_NEAR_Z,
                -ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z,
                -ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z,
                -ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_NEAR_Z,
                1, 0, 0);

            draw_flat_quad(
                ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z,
                ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_NEAR_Z,
                ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_NEAR_Z,
                ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z,
                -1, 0, 0);
        glEnd();
    }
}