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

void Triangle::Destroy() {
    destroyed = true;
}

void Triangle::Draw() {
    SetMVP();
    glBindVertexArray(vao);
    if (destroyed) {
        offset = offset - vec3(0, 0, downVelocity) * Time::DeltaTime();
        downVelocity += 0.6f * Time::DeltaTime();
        for (size_t i = 0; i < vtxData.size(); i+=3)
        {
            vec3 n1 = vtxData[i].norm;
            vec3 n2 = vtxData[i+1].norm;
            vec3 n3 = vtxData[i+2].norm;

            vec3 averageNormal = normalize((n1 + n2 + n3) / 3.0f);
            vec3 offs = (averageNormal* 0.3f)*Time::DeltaTime();
            
            vtxData[i].pos = vtxData[i].pos + offs;
            vtxData[i+1].pos = vtxData[i+1].pos + offs;
            vtxData[i+2].pos = vtxData[i+2].pos + offs;
        }
        Upload();
    }
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
