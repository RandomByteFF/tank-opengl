#pragma once

#include "geometry.h"

class Triangle : public Geometry {
	public:
	Triangle(Material* mat, vec3 pos, vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0));
    void Draw() override;
};