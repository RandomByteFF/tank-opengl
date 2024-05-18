#pragma once
#include "framework.h"
#include "material.h"
#include "camera.h"

struct VertexData {
	vec3 pos, norm;
	vec2 tex;
};

class Geometry {
	protected:
    Material* material;
	vec3 scale, rot, pos;

	unsigned int vao, vbo;
	std::vector<VertexData> vtxData;

	void SetMVP();

	public:
	vec3 offset, rOffset;

	Geometry(Material* material);

	//virtual void Create() = 0;

	virtual void Draw() = 0;

	virtual void Upload();

	virtual ~Geometry();
};