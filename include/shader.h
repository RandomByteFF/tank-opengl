#pragma once

#include "framework.h"
#include <fstream>
#include <iostream>
#include <exception>

class Shader { 
    public:
    static GPUProgram create(const std::string vertFile, const std::string fragFile, std::string output);
};