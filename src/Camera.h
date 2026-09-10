#pragma once
#include "Types.h"

struct Camera {
    Vec3 eye;
    Vec3 center;
    Vec3 up;
    float yaw = 0.0f;      
    float pitch = 0.0f;    
    float lookDistance = 100.0f;
};

void camera_init(Camera& cam, Vec3 eye, Vec3 up, float lookDistance);
void camera_apply(const Camera& cam);                       
void camera_handle_mouse(Camera& cam, int mouseX, int mouseY, int windowWidth, int windowHeight);
Vec3 camera_aim_direction(const Camera& cam);               