#pragma once
#include "gameObject.h"
#include "cylinder.h"
#include "triangle.h"
#include "circle.h"
#include "plane.h"
#include "bullet.h"
#include "phong.h"
#include "scene.h"

class Track;
class Tank : public GameObject {
    float speedRight = 0;
    float speedLeft = 0;
    float animLeft = 0;
    float animRight = 0;
    Material* bulletMat;

    void Rotate(float angle);
    void Move(float speed);

    bool feq(float a, float b) {
		return abs(a-b) < 0.0001;
	}

    void TurnRight(float rot);
    void TurnLeft(float rot);

    Track* rightTrack;
    Track* leftTrack;
    GameObject* turret;
    Cylinder* canon;

    public:
    bool destroyed = false;
    Tank(Material* mat);
    void Animate(float deltaTime) override;

    void ChangeSpeedRight(float speed);
    void ChangeSpeedLeft(float speed);
    void RotateTurret(float angle);
    void LiftCanon(float angle);

    vec3 GetFacing();
    vec3 GetLookDirection();
    void Shoot();
    void Draw(bool destroyed = false) override;
    void Destroy() override;
};

class Track : public GameObject{
    public:
    Track(Material* mat, bool right);
    void Animate(float t) override;
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