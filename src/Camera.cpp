// src/Camera.cpp
#include "Camera.h"
#include <GL/freeglut.h>
#include <cmath>

static constexpr float MAX_YAW = 60.0f;   // degrees, clamps left/right turn
static constexpr float MAX_PITCH = 40.0f; // degrees, clamps up/down look

namespace {
    float clampf(float v, float lo, float hi) {
        if (v < lo) return lo;
        if (v > hi) return hi;
        return v;
    }
}

void camera_init(Camera& cam, Vec3 eye, Vec3 up, float lookDistance) {
    cam.eye = eye;
    cam.up = up;
    cam.yaw = 0.0f;
    cam.pitch = 0.0f;
    cam.lookDistance = lookDistance;
    cam.center = camera_aim_direction(cam) * lookDistance + cam.eye;
}

Vec3 camera_aim_direction(const Camera& cam) {
    float yawRad = cam.yaw * TO_RAD;
    float pitchRad = cam.pitch * TO_RAD;
    // yaw=0,pitch=0 looks down -Z; positive yaw turns toward +X, positive pitch looks up (+Y)
    return Vec3(std::sin(yawRad) * std::cos(pitchRad),
                std::sin(pitchRad),
                -std::cos(yawRad) * std::cos(pitchRad));
}

void camera_apply(const Camera& cam) {
    gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z,
              cam.center.x, cam.center.y, cam.center.z,
              cam.up.x, cam.up.y, cam.up.z);
}

void camera_handle_mouse(Camera& cam, int mouseX, int mouseY, int windowWidth, int windowHeight) {
    float normX = static_cast<float>(mouseX - windowWidth / 2) / (windowWidth / 2.0f);
    cam.yaw = clampf(normX * MAX_YAW, -MAX_YAW, MAX_YAW);

    // Screen Y grows downward, so moving the mouse up (smaller mouseY) should
    // increase pitch (look up) — hence the negative sign.
    float normY = static_cast<float>(mouseY - windowHeight / 2) / (windowHeight / 2.0f);
    cam.pitch = clampf(-normY * MAX_PITCH, -MAX_PITCH, MAX_PITCH);

    cam.center = camera_aim_direction(cam) * cam.lookDistance + cam.eye;
}