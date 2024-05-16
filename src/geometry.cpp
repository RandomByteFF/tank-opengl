#include "geometry.h"

Geometry::Geometry(Material* material) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    this->material = material;
}

void Geometry::Draw() {
		material->getShader()->Use();

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

		material->Set();
	}

Geometry::~Geometry() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}