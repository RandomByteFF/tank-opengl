#include "phongText.h"

PhongText::PhongText(GPUProgram *shader) : Material(shader) {
    this->shader->Use();
    texture = std::vector<vec4>(100*100);
    for (size_t i = 0; i < 100*100; i++)
    {
        texture[i] = vec4((float)rand()/RAND_MAX/4, (float)rand()/RAND_MAX/2, (float)rand()/RAND_MAX/5, 1);
    }
    
    t.create(100, 100, texture, GL_LINEAR);
}

void PhongText::Set()
{
    shader->Use();
    shader->setUniform(vec3(0.1, 0.1, 0.1), "ks");
    shader->setUniform(vec3(0.2, 0.2, 0.2), "ka");
    shader->setUniform(100.f, "shine");
    shader->setUniform(vec3(1, 1, 1), "La");
    shader->setUniform(vec3(1, 1, 1), "Le");
}
