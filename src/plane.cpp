#include "plane.h"

Plane::Plane(Material * mat, vec3 pos, vec3 scale, vec3 rot) : ParamSurface(3, 3, mat) {
    this->pos = pos;
    this->scale = scale;
    this->rot = rot;
}

VertexData Plane::GenVertexData(float u, float v) {
    VertexData vd;
    vd.tex = vec2(u,v);
    vd.pos = vec3(u-0.5, v-0.5, 0);
    vd.norm = vec3(0, 0, 1);
    return vd;
}
