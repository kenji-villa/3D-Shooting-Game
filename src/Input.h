#pragma once

// Registered directly as freeglut callbacks; signatures must match exactly.
void input_passive_motion(int x, int y);
void input_mouse(int button, int state, int x, int y);
void input_key_up(unsigned char key, int x, int y);