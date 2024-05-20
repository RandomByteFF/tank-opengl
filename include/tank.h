#pragma once
#include "gameObject.h"
#include "cylinder.h"
#include "triangle.h"
#include "circle.h"
#include "plane.h"

class Tank : public GameObject{
    public:
    Tank(Material* mat);

    void RotateRight(float angle);
};

class Track : public GameObject{
    public:
    Track(Material* mat);
};

class TrackElement : public Plane {
	bool right = true;
	size_t num;
	public:

	TrackElement(Material* mat, bool right, size_t num) : Plane(mat, vec3(0,0,0), vec3(0.4, 0.1, 1)){
		this->right = right;
		this->num = num;
		Animate(0);
	}

	void Animate(float t);
};