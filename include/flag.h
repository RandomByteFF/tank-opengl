#pragma once
#include "paramSurface.h"

class Flag : public ParamSurface {
    public:
    float W = 1;
    float H = 1;
    float D = 0.1;
    float K = 10;
    float phase = 0;
    Flag(Material* mat, vec3 pos, vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0));
    VertexData GenVertexData(float u, float v) override;
    void AddPhase(float p);
};