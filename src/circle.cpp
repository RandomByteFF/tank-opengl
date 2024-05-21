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

void Circle::Destroy() {
    destroyed = true;
    std::vector<VertexData> newData = std::vector<VertexData>();

    for (size_t i = 1; i < vtxData.size(); i++)
    {
        newData.push_back(vtxData[0]);
        newData.push_back(vtxData[i]);
        newData.push_back(vtxData[i+1]);
    }
    
    vtxData = newData;
    spread = std::vector<vec3>(vtxData.size()/3);
    for (size_t i = 0; i < vtxData.size()/3; i++) {
        spread[i] = normalize(vec3(rand() % 100-50, rand() % 100-50, rand() % 100-50));
    }
    Upload();
}

void Circle::Draw() {
    SetMVP();
    glBindVertexArray(vao);
    if (!destroyed) {
        glDrawArrays(GL_TRIANGLE_FAN, 0, res+2);
    }
    else {
        offset = offset - vec3(0, 0, downVelocity) * Time::DeltaTime();
        downVelocity += 0.6f * Time::DeltaTime();
        for (size_t i = 0; i < vtxData.size(); i+=3)
        {
            vec3 n1 = vtxData[i].norm;
            vec3 n2 = vtxData[i+1].norm;
            vec3 n3 = vtxData[i+2].norm;


            vec3 averageNormal = normalize((n1 + n2 + n3) / 3.0f);
            vec3 offs = (averageNormal* 0.3f + spread[i/3] * 0.3f)*Time::DeltaTime();
            
            vtxData[i].pos = vtxData[i].pos + offs;
            vtxData[i+1].pos = vtxData[i+1].pos + offs;
            vtxData[i+2].pos = vtxData[i+2].pos + offs;

            Upload();
        }
        glDrawArrays(GL_TRIANGLES, 0, vtxData.size());
    }
}
