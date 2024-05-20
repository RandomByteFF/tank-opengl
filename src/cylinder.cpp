#include "cylinder.h"

Cylinder::Cylinder(Material * mat, vec3 pos, vec3 scale, vec3 rot) : ParamSurface(mat) {
    this->pos = pos;
    this->scale = scale;
    this->rot = rot;
    this->Create(2,30);
}

VertexData Cylinder::GenVertexData(float u, float v) {
    VertexData vd;
    vd.tex = vec2(u,v);
    vd.pos = vec3(cosf(2*M_PI*u)/2, sinf(2*M_PI*u)/2, v);
    vd.norm = normalize(vec3(vd.pos.x, vd.pos.y, 0));
    return vd;
}
