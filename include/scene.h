#pragma once
#include "gameObject.h"


class Scene {
    std::vector<GameObject*> objects;
    public:
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
    ~Scene() {
        for (GameObject* object : objects) {
            delete object;
        }
    }
};