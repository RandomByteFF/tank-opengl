#include "geometry.h"

void Geometry::SetMVP() {
	//material->getShader()->Use();
	material->Set();

	mat4 M = ScaleMatrix(scale) * 		
	RotationMatrix(rot.x + rOffset.x, vec3(1, 0, 0))*
	RotationMatrix(rot.y + rOffset.y, vec3(0, 1, 0)) *
	RotationMatrix(rot.z + rOffset.z, vec3(0, 0, 1))*
	TranslateMatrix(pos + offset);

	mat4 Minv = TranslateMatrix(-(pos+offset))*	
	RotationMatrix(-(rot.z + rOffset.z), vec3(0, 0, 1))*	
	RotationMatrix(-(rot.y + rOffset.y), vec3(0, 1, 0)) *
	RotationMatrix(-(rot.x + rOffset.x), vec3(1, 0, 0))*
	ScaleMatrix(vec3(1,1,1)/scale);

	mat4 MVP = M*Camera::GetInstance()->V()*Camera::GetInstance()->P();

	material->getShader()->setUniform(M, "M");
	material->getShader()->setUniform(Minv, "Minv");
	material->getShader()->setUniform(MVP, "MVP");
}

Geometry::Geometry(Material* material) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, norm));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tex));

    this->material = material;
}

void Geometry::Upload() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vtxData.size() * sizeof(VertexData), &vtxData[0], GL_STATIC_DRAW);
}

Geometry::~Geometry()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}