#include "tank.h"

void Tank::TurnRight(float rot) {
    vec3 facing = GetFacing();

    vec3 c = pos + normalize(vec3(facing.y, -facing.x, 0)) * 0.75;
    vec3 p_ = pos - c;
    vec4 p = vec4(p_.x, p_.y, p_.z, 1) * RotationMatrix(-rot, vec3(0, 0, 1));
    pos = vec3(p.x, p.y, p.z) + c;
}

void Tank::TurnLeft(float rot) {
    vec3 facing = GetFacing();

    vec3 c = pos + normalize(vec3(-facing.y, facing.x, 0)) * 0.75;
    vec3 p_ = pos - c;
    vec4 p = vec4(p_.x, p_.y, p_.z, 1) * RotationMatrix(rot, vec3(0, 0, 1));
    pos = vec3(p.x, p.y, p.z) + c;
}

Tank::Tank(Material *mat) : GameObject()
{
    GameObject* turret = new GameObject();
    Cylinder* c = new Cylinder(mat, vec3(0, 0, 0.3), vec3(1.1, 1.1, 0.8)); //body
    turret->AddPrimitive(c);

    Triangle* t = new Triangle(mat, vec3(0, 0, 1.1), vec3(1.5, 1.4422, 1), vec3(0.588, 0, 0)); //back plate
    AddPrimitive(t);

    t = new Triangle(mat, vec3(0, 0, 1.1), vec3(1.5, 1.4422, 1), vec3(0.588, 0, M_PI)); //front plate
    AddPrimitive(t);

    Circle* ci = new Circle(mat, vec3(0, 0, 1.1), vec3(1.1, 1.1, 1)); //top plate
    AddPrimitive(ci);

    Cylinder* canon = new Cylinder(mat, vec3(0, 0.3, 0.9), vec3(0.2, 0.2, 1.5), vec3(M_PI/2*3, 0, 0)); //canon
    turret->AddPrimitive(canon);
    AddChild(turret);

    Track* tr = new Track(mat);
    AddChild(tr);
}

void Tank::Animate(float deltaTime) {

    TurnRight(1.5*speedLeft * deltaTime);
    TurnLeft(1.5*speedRight * deltaTime);
    Rotate(1.5*speedRight * deltaTime);
    Rotate(-1.5*speedLeft * deltaTime);
    if (feq(speedRight+speedLeft, 0)) {
        //Move(speedRight * deltaTime);
    }
    else if (speedRight > 0 && speedLeft > 0) {
        Move((speedRight < speedLeft ? speedRight : speedLeft) * deltaTime);
    }
    else if (speedRight < 0 && speedLeft < 0) {
        Move((speedRight > speedLeft ? speedRight : speedLeft) * deltaTime);
    }
    else {
        Move((abs(speedRight) < abs(speedLeft) ? speedRight : speedLeft) * deltaTime);
    }
}

void Tank::ChangeSpeedRight(float speed) {
    speedRight += speed;
}

void Tank::ChangeSpeedLeft(float speed) {
    speedLeft += speed;
}

vec3 Tank::GetFacing() {
    return normalize(vec3(-sinf(rot.z), cosf(rot.z), 0));
}

void Tank::Rotate(float angle) {
    rot = vec3(0, 0, rot.z + angle);
}

void Tank::Move(float speed) {
    pos = pos + vec3(-speed * sinf(rot.z), speed * cosf(rot.z), 0);
}

Track::Track(Material *mat) {
    for (size_t i = 0; i < 36; i++) {
        TrackElement* t = new TrackElement(mat, true, i);
        AddPrimitive(t);
    }
    for (size_t i = 0; i < 36; i++) {
        TrackElement* t = new TrackElement(mat, false, i);
        AddPrimitive(t);
    }
}
void TrackElement::Animate(float t) {
    //0-1.8
    //1.8-2.7
    //2.7-4.5
    //4.5-5.4
    t += num*0.15;
    if (t < 0) t = 5.4+t;
    float n = fmod(t, 5.4);
    vec3 d;
    if (n < 1.8) {
        rot = vec3(0,0,0);
        d = vec3(0, n, 0);
    }
    else if (n < 2.7) {
        float dl = n - 1.8;
        rot = vec3(-dl/0.3, 0, 0);
        d = vec3(0, 1.8 + 0.3* sinf(dl/0.3), -0.3*(1-cosf(dl/0.3)));
    }
    else if (n < 4.5) {
        rot = vec3(M_PI, 0, 0);
        d = vec3(0, 4.5-n, -0.6);
    }
    else {
        float dl = 5.4-n;
        rot = vec3(dl/0.3, 0, 0);
        d = vec3(0, - 0.3* sinf(dl/0.3), -0.3*(1-cosf(dl/0.3)));
    }

    pos = d + vec3((right ?  1: -1)*0.75, -0.9, 0.6);
}
