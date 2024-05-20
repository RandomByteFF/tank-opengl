#pragma once

#include "geometry.h"

class ParamSurface : public Geometry {
    protected:
	unsigned int nVtxStrip, nStrips;
	virtual VertexData GenVertexData(float u, float v) = 0;

	public:
	ParamSurface(Material* material) : Geometry(material) {};
	void Create(size_t N, size_t M);

	void Draw() override;
};