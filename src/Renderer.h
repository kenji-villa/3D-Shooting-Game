#pragma once
#include "Target.h"
#include "Projectile.h"

void renderer_setup_lights();
void renderer_draw_scene(const Target& target, const Projectile& proj);