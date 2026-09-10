// src/Hud.cpp
#include "Hud.h"
#include "Types.h"
#include <GL/freeglut.h>
#include <cstdio>
#include <cstring>

namespace {
    void draw_text(float x, float y, const char* text, void* font) {
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; ++c) {
            glutBitmapCharacter(font, *c);
        }
    }

    // Rough width estimates for right/center-aligning text without a real
    // font-metrics lookup — close enough for HUD purposes.
    float estimate_width(const char* text, float avgCharWidth) {
        return static_cast<float>(std::strlen(text)) * avgCharWidth;
    }

    void draw_score_corner(int score, int windowWidth, int windowHeight) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "Score: %d", score);

        float margin = 20.0f;
        float x = windowWidth - estimate_width(buf, 10.0f) - margin;
        float y = windowHeight - margin;

        glColor3f(1.0f, 1.0f, 1.0f);
        draw_text(x, y, buf, GLUT_BITMAP_HELVETICA_18);
    }

    void draw_scope(int windowWidth, int windowHeight) {
        float cx = windowWidth / 2.0f;
        float cy = windowHeight / 2.0f;
        float innerGap = 4.0f;
        float tickLength = 8.0f;
        float ringRadius = 16.0f;

        glColor3f(0.15f, 0.95f, 0.15f);
        glLineWidth(2.0f);

        glBegin(GL_LINES);
            glVertex2f(cx - tickLength - innerGap, cy);
            glVertex2f(cx - innerGap, cy);
            glVertex2f(cx + innerGap, cy);
            glVertex2f(cx + innerGap + tickLength, cy);
            glVertex2f(cx, cy - tickLength - innerGap);
            glVertex2f(cx, cy - innerGap);
            glVertex2f(cx, cy + innerGap);
            glVertex2f(cx, cy + innerGap + tickLength);
        glEnd();

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 48; ++i) {
            float angle = (static_cast<float>(i) / 48.0f) * 2.0f * PI;
            glVertex2f(cx + std::cos(angle) * ringRadius,
                       cy + std::sin(angle) * ringRadius);
        }
        glEnd();
    }

    void draw_centered(float cy, int windowWidth, const char* text, void* font, float avgCharWidth) {
        float width = estimate_width(text, avgCharWidth);
        float x = (windowWidth - width) / 2.0f;
        draw_text(x, cy, text, font);
    }

    void draw_game_over_overlay(int score, int windowWidth, int windowHeight) {
        // Dim the whole scene behind the modal so the text/menu reads clearly.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.65f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(static_cast<float>(windowWidth), 0.0f);
            glVertex2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
            glVertex2f(0.0f, static_cast<float>(windowHeight));
        glEnd();
        glDisable(GL_BLEND);

        float centerY = windowHeight / 2.0f;

        glColor3f(0.95f, 0.15f, 0.15f);
        draw_centered(centerY + 50.0f, windowWidth, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24, 14.0f);

        char scoreBuf[48];
        std::snprintf(scoreBuf, sizeof(scoreBuf), "Final Score: %d", score);
        glColor3f(1.0f, 1.0f, 1.0f);
        draw_centered(centerY, windowWidth, scoreBuf, GLUT_BITMAP_HELVETICA_18, 10.0f);

        glColor3f(0.85f, 0.85f, 0.85f);
        draw_centered(centerY - 40.0f, windowWidth, "Click, or press N, to Play Again",
                      GLUT_BITMAP_HELVETICA_18, 10.0f);
    }
}

void hud_draw(const GameState& gs, int windowWidth, int windowHeight) {
    // Switch to a 2D orthographic overlay: (0,0) bottom-left, (w,h) top-right,
    // matching window pixels 1:1.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    draw_score_corner(gs.score, windowWidth, windowHeight);

    if (gs.mode == GameMode::GameOver) {
        draw_game_over_overlay(gs.score, windowWidth, windowHeight);
    } else {
        draw_scope(windowWidth, windowHeight); // no need to aim once the round's over
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}