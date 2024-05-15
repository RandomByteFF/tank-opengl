#pragma once

#include "framework.h"
#include <fstream>
#include <iostream>
#include <exception>

class Shader : public GPUProgram{ 
    public:
    //GPUProgram program;
    void createShader(const std::string vertFile, const std::string fragFile, std::string output);
};