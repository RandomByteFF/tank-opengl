#pragma once

#include "geometry.h"

class ParamSurface : public Geometry {
    protected:
	unsigned int nVtxStrip, nStrips;
	virtual VertexData GenVertexData(float u, float v) = 0;

	public:
	ParamSurface(size_t N, size_t M, Material* material);

	void Draw() override;
};