#pragma once

#include "paramSurface.h"

class Plane : public ParamSurface {
	public:
	Plane(Material* mat, vec3 pos, vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0));
	VertexData GenVertexData(float u, float v) override;
};