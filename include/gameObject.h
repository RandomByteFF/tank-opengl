#pragma once
#include "geometry.h"

class GameObject { 
    static vec3 RotateAroundPoint(vec3 pivot, vec3 point, vec3 axis, float angle);
    protected:
    std::vector<Geometry*> primitives;
    std::vector<GameObject*> children;

    public:
    vec3 pos, rot, scale;
    GameObject(vec3 pos = vec3(0,0,0), vec3 scale = vec3(1,1,1), vec3 rot = vec3(0,0,0)) : pos(pos), rot(rot), scale(scale) {}

    void AddPrimitive(Geometry* primitive) {
        primitives.push_back(primitive);
    }

    void AddChild(GameObject* child) {
        children.push_back(child);
    }

    virtual void Draw(bool destroyed = false);
    virtual void Draw(vec3 parentPos, vec3 parentRot, vec3 parentScale, bool destroyed = false);

    virtual void Animate(float deltaTime) {
        for (GameObject* g : children) {
            g->Animate(deltaTime);
        }
    }

    virtual void Destroy() {
        for (Geometry* g : primitives) {
            g->Destroy();
        }
        for (GameObject* g : children) {
            g->Destroy();
        }
    }

    virtual ~GameObject() {
        for (Geometry* g : primitives) {
            delete g;
        }
        for (GameObject* g : children) {
            delete g;
        }
    }
};