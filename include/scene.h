#pragma once
#include "gameObject.h"
#include "tank.h"

class Tank;
class Scene {
    std::vector<GameObject*> objects;
    std::vector<Tank*> tanks;
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

    void AddTank(Tank* tank) {
        objects.push_back((GameObject*)tank);
        tanks.push_back(tank);
    }

    Tank* GetTank(size_t index) {
        return tanks[index];
    }

    size_t TankCount() {
        return tanks.size();
    }

    ~Scene() {
        for (GameObject* object : objects) {
            delete object;
        }
    }
};