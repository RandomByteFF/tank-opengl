#include "gameObject.h"

void GameObject::Draw() {
    for (Geometry* g : primitives) {
        g->rOffset = rot;
        g->offset = pos;
        g->Draw();
    }
    for (GameObject* g : children) {
        g->Draw(pos, rot, scale);
    }
}

void GameObject::Draw(vec3 parentPos, vec3 parentRot, vec3 parentScale) {
    for (Geometry* g : primitives) {
        g->rOffset = rot + parentRot;
        g->offset = pos + parentPos;
        g->Draw();
    }
    for (GameObject* g : children) {
        g->Draw(pos + parentPos, rot + parentRot, scale * parentScale);
    }
}
