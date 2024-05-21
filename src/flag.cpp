#include "flag.h"

Flag::Flag(Material *mat, vec3 pos, vec3 scale, vec3 rot) : ParamSurface(mat){
    this->pos = pos;
    this->scale = scale;
    this->rot = rot;
    this->Create(2,20);
}

VertexData Flag::GenVertexData(float u, float v) {
    VertexData vd;
    vd.tex = vec2(u, v);
    float angle = u * K + phase;
    vd.pos = vec3(u * W, v * H, D * sin(angle));
    vd.norm = vec3(-K * D * cos(angle), 0, W);
    return vd;
}

void Flag::AddPhase(float p) {
    phase += p;
    this->Create(2,20);
}
