#include "paramSurface.h"

void ParamSurface::Create(size_t N, size_t M) {
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

void ParamSurface::Draw()
{
    SetMVP();
    glBindVertexArray(vao);
    for (size_t i = 0; i < nStrips; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i*nVtxStrip, nVtxStrip);
    }
}
