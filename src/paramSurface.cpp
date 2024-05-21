#include "paramSurface.h"

void ParamSurface::Create(size_t N, size_t M) {
    this->N = N;
    this->M = M;
    nVtxStrip = (M + 1) * 2;
    nStrips = N;
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j <= M; j++)
        {
            vtxData.push_back(GenVertexData((float) j/M, (float) i/N));
            vtxData.push_back(GenVertexData((float) j/M, (float) (i+1)/N));
        }
    }
    Upload();
}

void ParamSurface::Destroy() {
    destroyed = true;
    std::vector<VertexData> newData = std::vector<VertexData>();

    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < M; j++)
        {
            newData.push_back(GenVertexData((float) j/M, (float) i/N));
            newData.push_back(GenVertexData((float) j/M, (float) (i+1)/N));
            newData.push_back(GenVertexData((float) (j+1)/M, (float) i/N));

            newData.push_back(GenVertexData((float) j/M, (float) (i+1)/N));
            newData.push_back(GenVertexData((float) (j+1)/M, (float) i/N));
            newData.push_back(GenVertexData((float) (j+1)/M, (float) (i+1)/N));
        }
    }
    vtxData = newData;
    spread = std::vector<vec3>(vtxData.size()/3);

    for (size_t i = 0; i < vtxData.size()/3; i++) {
        spread[i] = normalize(vec3(rand() % 100, rand() % 100, rand() % 100));
    }

    Upload();
}

void ParamSurface::Draw()
{
    SetMVP();
    glBindVertexArray(vao);
    if (!destroyed) {
        for (size_t i = 0; i < nStrips; i++) {
            glDrawArrays(GL_TRIANGLE_STRIP, i*nVtxStrip, nVtxStrip);
        }
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
            vec3 offs = averageNormal* 0.3f * Time::DeltaTime();
            
            vtxData[i].pos = vtxData[i].pos + offs + spread[i/3];
            vtxData[i+1].pos = vtxData[i+1].pos + offs + spread[i/3];
            vtxData[i+2].pos = vtxData[i+2].pos + offs + spread[i/3];

            Upload();
        }
        
        glDrawArrays(GL_TRIANGLES, 0, vtxData.size());
    }
}
