#include "phongText.h"

void PhongText::Set() {
    shader->setUniform(vec3(0.1, 0.1, 0.1), "ks");
    shader->setUniform(vec3(0.2, 0.2, 0.2), "ka");
    shader->setUniform(100.f, "shine");
    shader->setUniform(vec3(1, 1, 1), "La");
    shader->setUniform(vec3(1, 1, 1), "Le");
}
