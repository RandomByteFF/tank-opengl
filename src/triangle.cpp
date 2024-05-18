#include "triangle.h"

Triangle::Triangle(Material * mat, vec3 pos, vec3 scale, vec3 rot) : Geometry(mat) {
    this->pos = pos;
    this->scale = scale;
    this->rot = rot;
    
    std::vector<VertexData> d(3);
    d[0].pos = vec3(0, 0, 0);
    d[0].norm = vec3(0, 0, 1);
    d[0].tex = vec2(0.5, 1);

    d[1].pos = vec3(-0.5, -1, 0);
    d[1].norm = vec3(0,0,1);
    d[1].tex = vec2(0, 0);

    d[2].pos = vec3(0.5, -1, 0);
    d[2].norm = vec3(0,0,1);
    d[2].tex = vec2(1, 0);
    
    Upload();
}

void Triangle::Draw() {
    SetMVP();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
