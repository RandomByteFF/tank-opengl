#include "circle.h"

Circle::Circle(Material * mat, vec3 pos, vec3 scale, size_t res) : Geometry(mat) {
    this->pos = pos;
    this->scale = scale;
    this->res = res;
    
    vtxData = std::vector<VertexData>(res+2);
    vtxData[0].norm = vec3(0,0,1);
    vtxData[0].pos = vec3(0,0,0);
    vtxData[0].tex = vec2(0.5, 0.5);
    for (size_t i = 0; i < res+1; i++) 
    {
        vtxData[i+1].norm = vec3(0,0,1);
        vtxData[i+1].pos = vec3(cosf(2*M_PI/res*i)/2, sinf(2*M_PI/res*i)/2, 0);
    }

    Upload();
}

void Circle::Draw() {
    SetMVP();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, res+2);
}
