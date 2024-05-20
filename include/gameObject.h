#pragma once
#include "geometry.h"

class GameObject { 
    protected:
    std::vector<Geometry*> primitives;
    std::vector<GameObject*> children;
    vec3 pos, rot, scale;

    public:
    GameObject(vec3 pos = vec3(0,0,0), vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0)) : pos(pos), rot(rot), scale(scale) {}

    void AddPrimitive(Geometry* primitive) {
        primitives.push_back(primitive);
    }

    void AddChild(GameObject* child) {
        children.push_back(child);
    }

    void Draw();
    void Draw(vec3 parentPos, vec3 parentRot, vec3 parentScale);
    ~GameObject() {
        for (Geometry* g : primitives) {
            delete g;
        }
        for (GameObject* g : children) {
            delete g;
        }
    }
};