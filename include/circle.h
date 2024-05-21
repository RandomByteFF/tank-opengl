#pragma once

#include "geometry.h"
#include "timeManager.h"

class Circle : public Geometry {
	bool destroyed = false;
	float downVelocity = 0;
	std::vector<vec3> spread;
	size_t res;
	public:
	Circle(Material* mat, vec3 pos, vec3 scale = vec3(1,1,1), size_t res = 30);
	void Destroy() override;

	void Draw();
};