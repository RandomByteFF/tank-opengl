#include "phong.h"

void Phong::Set() {
    shader->Use();
    shader->setUniform(vec3(0.4, 0.4, 0.4), "kd");
    shader->setUniform(vec3(1, 1, 1), "ks");
    shader->setUniform(vec3(0.2, 0.2, 0.2), "ka");
    shader->setUniform(5.f, "shine");
    shader->setUniform(vec3(1, 1, 1), "La");
    shader->setUniform(vec3(1, 1, 1), "Le");
}