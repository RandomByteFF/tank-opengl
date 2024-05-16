#pragma once

#include "framework.h"
#include <fstream>
#include <iostream>
#include <exception>

class Shader : public GPUProgram { 
    static std::vector<Shader*> shaders;
    public:
    void createShader(const std::string vertFile, const std::string fragFile, std::string output);

    static Shader* const getShader(int index) {
        return shaders[index];
    }

    static size_t getShaderCount() {
        return shaders.size();
    }
};