#include "shader.h"

std::vector<Shader*> Shader::shaders;

void Shader::createShader(const std::string vertFile, const std::string fragFile, std::string output) {
    std::string vertSource;
    std::string fragSource;

    try {
        std::ifstream vertReader(vertFile);
        std::ifstream fragReader(fragFile);

        std::string line;
        while(std::getline(vertReader, line)){
            vertSource.append(line + '\n');
        }
        while(std::getline(fragReader, line)){
            fragSource.append(line + '\n');
        }

        vertReader.close();
        fragReader.close();
    }
    catch (std::exception e){
        throw e;
    }
    GPUProgram::create(vertSource.c_str(), fragSource.c_str(), output.c_str());
    shaders.push_back(this);
}

Shader * const Shader::getShader(int index) {
    return shaders[index];
}

size_t Shader::getShaderCount() {
    return shaders.size();
}
