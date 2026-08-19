// src/Hud.cpp
#include "Hud.h"
#include "Types.h"
#include <GL/freeglut.h>
#include <cstdio>
#include <cstring>

namespace {
    void draw_text(float x, float y, const char* text) {
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }

    // Rough width estimate for right-aligning text without a font-metrics
    // lookup — Helvetica18 averages ~10px/char, good enough for a HUD label.
    float estimate_text_width(const char* text) {
        return static_cast<float>(std::strlen(text)) * 10.0f;
    }

    void draw_score(int score, int windowWidth, int windowHeight) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "Score: %d", score);

        float margin = 20.0f;
        float x = windowWidth - estimate_text_width(buf) - margin;
        float y = windowHeight - margin;

        glColor3f(1.0f, 1.0f, 1.0f);
        draw_text(x, y, buf);
    }

    void draw_scope(int windowWidth, int windowHeight) {
        float cx = windowWidth / 2.0f;
        float cy = windowHeight / 2.0f;
        float innerGap = 4.0f;
        float tickLength = 8.0f;
        float ringRadius = 16.0f;

        glColor3f(0.15f, 0.95f, 0.15f);
        glLineWidth(2.0f);

        // Crosshair ticks (gap in the middle so the ring reads clearly)
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

        // Scope ring
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 48; ++i) {
            float angle = (static_cast<float>(i) / 48.0f) * 2.0f * PI;
            glVertex2f(cx + std::cos(angle) * ringRadius,
                       cy + std::sin(angle) * ringRadius);
        }
        glEnd();
    }
}

void hud_draw(int score, int windowWidth, int windowHeight) {
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

    draw_score(score, windowWidth, windowHeight);
    draw_scope(windowWidth, windowHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}