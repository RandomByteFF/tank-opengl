#include "shader.h"

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
        printf(vertSource.c_str());
        fragReader.close();
        printf(fragSource.c_str());
    }
    catch (std::exception e){
        throw e;
    }
    GPUProgram::create(vertSource.c_str(), fragSource.c_str(), output.c_str());
    //return this;
}