#pragma once

#include "framework.h"
#include "material.h"

class PhongText : public Material {
    Texture t;
    std::vector<vec4> texture;
    public:
    PhongText(GPUProgram* shader);
    void Set() override;
};