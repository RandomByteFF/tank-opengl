#pragma once
#include "gameObject.h"


class Scene {
    std::vector<GameObject*> objects;
    static Scene* active;
    public:
    Scene() {
        active = this;
    }
    void AddObject(GameObject* object) {
        objects.push_back(object);
    }

    void Render(float deltaTime) {
        for (GameObject* object : objects) {
            object->Animate(deltaTime);
        }
        
        for (GameObject* object : objects) {
            object->Draw();
        }
    }

    static Scene* Get() {
        return active;
    }

    ~Scene() {
        for (GameObject* object : objects) {
            delete object;
        }
    }
};