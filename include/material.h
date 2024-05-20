#pragma once
#include "framework.h"

//TODO: material shouldn't set itself if it's already in use
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