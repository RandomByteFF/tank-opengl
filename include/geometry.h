#pragma once
#include "framework.h"
#include "material.h"
#include "camera.h"

class Drawable {
	public:
	virtual void Draw() = 0;
};

class Geometry : public Drawable{
	protected:
    Material* material;
	vec3 scale, rot, pos;

	unsigned int vao, vbo;

	public:
	vec3 offset, rOffset;

	struct VertexData {
		vec3 pos, norm;
		vec2 tex;
	};

	Geometry(Material* material);

	virtual void Draw();

	virtual ~Geometry();
};