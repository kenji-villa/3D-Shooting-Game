#include "Room.h"
#include <GL/freeglut.h>

void room_draw() {
    // Floor
    glColor3f(0.55f, 0.55f, 0.55f);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(-ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_NEAR_Z);
        glVertex3f( ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_NEAR_Z);
        glVertex3f( ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_FAR_Z);
        glVertex3f(-ROOM_HALF_WIDTH, ROOM_FLOOR_Y, ROOM_FAR_Z);
    glEnd();

    // Back wall
    glColor3f(0.65f, 0.65f, 0.7f);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(-ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z);
        glVertex3f( ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z);
        glVertex3f( ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z);
        glVertex3f(-ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z);
    glEnd();

    // Left wall
    glColor3f(0.6f, 0.6f, 0.65f);
    glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(-ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_NEAR_Z);
        glVertex3f(-ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z);
        glVertex3f(-ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z);
        glVertex3f(-ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_NEAR_Z);
    glEnd();

    // Right wall
    glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_FAR_Z);
        glVertex3f(ROOM_HALF_WIDTH, ROOM_FLOOR_Y,   ROOM_NEAR_Z);
        glVertex3f(ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_NEAR_Z);
        glVertex3f(ROOM_HALF_WIDTH, ROOM_CEILING_Y, ROOM_FAR_Z);
    glEnd();
}