#include "gameObject.h"

vec3 GameObject::RotateAroundPoint(vec3 pivot, vec3 point, vec3 axis, float angle) {
    point = point - pivot;
    mat4 rotMat = RotationMatrix(angle, axis);
    vec4 rotatedPos = vec4(point.x, point.y, point.z, 1.0f) * rotMat;
    return vec3(rotatedPos.x, rotatedPos.y, rotatedPos.z) + pivot;
}

void GameObject::Draw()
{
    for (Geometry* g : primitives) {
        vec3 p = this->pos + g->getPos();
        p = RotateAroundPoint(this->pos, p, vec3(0,0,1), rot.z);
        p = RotateAroundPoint(this->pos, p, vec3(0,1,0), rot.y);
        p = RotateAroundPoint(this->pos, p, vec3(1,0,0), rot.x);
        g->rOffset = rot;
        g->offset = p - g->getPos();
        g->Draw();
    }
    
    for (GameObject* g : children) {
        g->Draw(pos, rot, scale);
    }
}

void GameObject::Draw(vec3 parentPos, vec3 parentRot, vec3 parentScale) {
    for (Geometry* g : primitives) {
        vec3 p = parentPos + this->pos + g->getPos();
        p = RotateAroundPoint(parentPos+this->pos, p, vec3(0,0,1), parentRot.z);
        p = RotateAroundPoint(parentPos+this->pos, p, vec3(0,1,0), parentRot.y);
        p = RotateAroundPoint(parentPos+this->pos, p, vec3(1,0,0), parentRot.x);
        g->rOffset = rot + parentRot;
        g->offset = p - g->getPos();
        g->Draw();
    }
    
    for (GameObject* g : children) {
        g->Draw(pos, rot+parentRot, scale);
    }
}
