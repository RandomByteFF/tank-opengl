#pragma once
#include "framework.h"

class Material {
    protected:
    GPUProgram* shader;

    public:
    Material(GPUProgram* shader) {
        this->shader = shader;
    }

    GPUProgram* getShader() {
        return shader;
    }

    virtual void Set() = 0;
};