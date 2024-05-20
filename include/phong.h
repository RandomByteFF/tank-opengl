#pragma once

#include "framework.h"
#include "material.h"

class Phong : public Material {
    public:
    using Material::Material;
    void Set() override;
};