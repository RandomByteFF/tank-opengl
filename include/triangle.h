#pragma once

#include "geometry.h"
#include "timeManager.h"

class Triangle : public Geometry {
	bool destroyed = false;
	float downVelocity = 0;
	public:
	Triangle(Material* mat, vec3 pos, vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0));
	void Destroy() override;

    void Draw() override;
};