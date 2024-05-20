#include "triangle.h"

Triangle::Triangle(Material * mat, vec3 pos, vec3 scale, vec3 rot) : Geometry(mat) {
    this->pos = pos;
    this->scale = scale;
    this->rot = rot;
    
    vtxData = std::vector<VertexData>(3);
    vtxData[0].pos = vec3(0, 0, 0);
    vtxData[0].norm = vec3(0, 0, 1);
    vtxData[0].tex = vec2(0.5, 1);

    vtxData[1].pos = vec3(-0.5, -1, 0);
    vtxData[1].norm = vec3(0,0,1);
    vtxData[1].tex = vec2(0, 0);

    vtxData[2].pos = vec3(0.5, -1, 0);
    vtxData[2].norm = vec3(0,0,1);
    vtxData[2].tex = vec2(1, 0);
    
    Upload();
}

void Triangle::Draw() {
    SetMVP();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
