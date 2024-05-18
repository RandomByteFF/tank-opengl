#pragma once

#include "geometry.h"

class Circle : public Geometry {
	size_t res;
	public:
	Circle(Material* mat, vec3 pos, vec3 scale = vec3(1,1,1), size_t res = 30);

	void Draw();
};