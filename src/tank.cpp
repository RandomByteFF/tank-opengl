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
    turret = new GameObject();
    Cylinder* c = new Cylinder(mat, vec3(0, 0, 0.3), vec3(1.1, 1.1, 0.8)); //body
    turret->AddPrimitive(c);

    Triangle* t = new Triangle(mat, vec3(0, 0, 1.1), vec3(1.5, 1.4422, 1), vec3(0.588, 0, 0)); //back plate
    AddPrimitive(t);

    t = new Triangle(mat, vec3(0, 0, 1.1), vec3(1.5, 1.4422, 1), vec3(0.588, 0, M_PI)); //front plate
    AddPrimitive(t);

    Circle* ci = new Circle(mat, vec3(0, 0, 1.1), vec3(1.1, 1.1, 1)); //top plate
    AddPrimitive(ci);

    canon = new Cylinder(mat, vec3(0, 0.3, 0.9), vec3(0.2, 0.2, 1.5), vec3(M_PI/2*3, 0, 0)); //canon
    turret->AddPrimitive(canon);
    AddChild(turret);

    rightTrack = new Track(mat, true);
    leftTrack = new Track(mat, false);
    AddChild(rightTrack);
    AddChild(leftTrack);

    bulletMat = mat;
}

void Tank::Animate(float deltaTime) {
    if (destroyed) return;
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

    animRight += speedRight * deltaTime;
    animLeft += speedLeft * deltaTime;

    rightTrack->Animate(animRight);
    leftTrack->Animate(animLeft);
}

void Tank::ChangeSpeedRight(float speed) {
    speedRight += speed;
}

void Tank::ChangeSpeedLeft(float speed) {
    speedLeft += speed;
}

void Tank::RotateTurret(float angle) {
    turret->rot = vec3(0, 0, turret->rot.z + angle);
}

void Tank::LiftCanon(float angle) {
    canon->rot = vec3(canon->rot.x + angle, 0, 0);
}

vec3 Tank::GetFacing() {
    return normalize(vec3(-sinf(rot.z), cosf(rot.z), 0));
}

vec3 Tank::GetLookDirection() {
    float angle = turret->rot.z;
    vec3 facing = GetFacing();
    vec4 f = vec4(facing.x, facing.y, facing.z, 1) * RotationMatrix(angle, vec3(0, 0, 1));
    return vec3(f.x, f.y, f.z);
}

void Tank::Shoot() {
    //magic
    vec3 look = GetLookDirection();
    vec3 perp = normalize(vec3(-look.y, look.x, 0));
    vec4 lookRotated = vec4(look.x, look.y, look.z, 1) * RotationMatrix(-canon->rot.x-M_PI/2, perp);
    vec3 rotatedLook = vec3(lookRotated.x, lookRotated.y, lookRotated.z);

    Bullet* b = new Bullet(bulletMat, pos + rotatedLook*1.5 + look*0.3 + vec3(0,0,0.9), vec3(0.2, 0.2, 0.2), rotatedLook*10, this);
    Scene::Get()->AddObject(b);
}

void Tank::Draw(bool destroyed) {
    GameObject::Draw(this->destroyed);
}

void Tank::Rotate(float angle) {
    rot = vec3(0, 0, rot.z + angle);
}

void Tank::Move(float speed) {
    pos = pos + vec3(-speed * sinf(rot.z), speed * cosf(rot.z), 0);
}

Track::Track(Material *mat, bool right) : GameObject() {
    for (size_t i = 0; i < 36; i++) {
        TrackElement* t = new TrackElement(mat, right, i);
        AddPrimitive(t);
    }
}
void Track::Animate(float t){
    for (Geometry* g : primitives) {
        ((TrackElement*)g)->Animate(t);
    }
}

void Tank::Destroy() {
    if (destroyed) return;
    destroyed = true;
    printf("hit");
    GameObject::Destroy();
}

void TrackElement::Animate(float t)
{
    //0-1.8
    //1.8-2.7
    //2.7-4.5
    //4.5-5.4
    t += num*0.15;
    if (t < 0) t += -floor(t/5.4)*5.4;
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
