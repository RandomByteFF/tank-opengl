#include "tank.h"

Tank::Tank(Material *mat) : GameObject() {
    Cylinder* c = new Cylinder(mat, vec3(0, 0, 0.3), vec3(1.1, 1.1, 0.8)); //body
    AddPrimitive(c);

    Triangle* t = new Triangle(mat, vec3(0, 0, 1.1), vec3(1.5, 1.4422, 1), vec3(0.588, 0, 0)); //back plate
    AddPrimitive(t);

    t = new Triangle(mat, vec3(0, 0, 1.1), vec3(1.5, 1.4422, 1), vec3(0.588, 0, M_PI)); //front plate
    AddPrimitive(t);

    Circle* ci = new Circle(mat, vec3(0, 0, 1.1), vec3(1.1, 1.1, 1)); //top plate
    AddPrimitive(ci);

    Track* tr = new Track(mat);
    AddChild(tr);
}

void Tank::RotateRight(float angle) {
    rot = vec3(0, 0, rot.z + angle);
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
    // vec4 r = vec4(toRot.x, toRot.y, toRot.z, 1) * RotationMatrix(angle, vec3(0,0,1));
    // pos = vec3(r.x, r.y, r.z) + tPos;

    // rot = rot + vec3(0, 0, angle);
}

