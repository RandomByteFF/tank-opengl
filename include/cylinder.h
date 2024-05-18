#pragma once

#include "paramSurface.h"

class Cylinder : public ParamSurface {
	public:
	Cylinder(Material* mat, vec3 pos, vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0));
	VertexData GenVertexData(float u, float v);
};