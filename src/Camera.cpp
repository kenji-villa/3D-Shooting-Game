#include "Camera.h"
#include <GL/freeglut.h>
#include <cmath>

static constexpr float MAX_YAW = 60.0f; // degrees, clamps how far you can turn

void camera_init(Camera& cam, Vec3 eye, Vec3 up, float lookDistance) {
    cam.eye = eye;
    cam.up = up;
    cam.yaw = 0.0f;
    cam.lookDistance = lookDistance;
    cam.center = camera_aim_direction(cam) * lookDistance + cam.eye;
}

Vec3 camera_aim_direction(const Camera& cam) {
    float rad = cam.yaw * TO_RAD;
    // yaw = 0 looks down -Z; positive yaw turns toward +X
    return Vec3(std::sin(rad), 0.0f, -std::cos(rad));
}

void camera_apply(const Camera& cam) {
    gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z,
              cam.center.x, cam.center.y, cam.center.z,
              cam.up.x, cam.up.y, cam.up.z);
}

void camera_handle_mouse(Camera& cam, int mouseX, int windowWidth) {
    float centered = static_cast<float>(mouseX - windowWidth / 2);
    float normalized = centered / (windowWidth / 2.0f); // -1 .. 1
    cam.yaw = normalized * MAX_YAW;

    if (cam.yaw > MAX_YAW) cam.yaw = MAX_YAW;
    if (cam.yaw < -MAX_YAW) cam.yaw = -MAX_YAW;

    cam.center = camera_aim_direction(cam) * cam.lookDistance + cam.eye;
}