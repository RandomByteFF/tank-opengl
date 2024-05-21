#pragma once

#include "geometry.h"
#include "timeManager.h"

class ParamSurface : public Geometry {
	float downVelocity = 0.0f;
	std::vector<vec3> spread;
    protected:
	size_t N, M;
	unsigned int nVtxStrip, nStrips;
	virtual VertexData GenVertexData(float u, float v) = 0;
	bool destroyed = false;

	public:
	ParamSurface(Material* material) : Geometry(material) {};
	void Create(size_t N, size_t M);

	void Destroy() override;
	void Draw() override;
};