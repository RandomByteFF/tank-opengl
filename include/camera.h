#pragma once
#include "framework.h"
#include "shader.h"

class Camera {
	vec3 wEye, wLookat, wVup;
	float fov, asp, fp, bp;
    static Camera* camera;
	public:
	Camera();

	mat4 V();
	mat4 P();

	vec3 GetEye() {
		return wEye;
	}
	void setTarget(vec3 target, vec3 facing);
    static Camera* const GetInstance();
};