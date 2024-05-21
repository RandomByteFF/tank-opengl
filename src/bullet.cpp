#include "bullet.h"

Bullet::Bullet(Material *material, vec3 pos, vec3 scale, vec3 velocity, Tank* source) : GameObject(pos) {
    this->source = source;
    this->velocity = velocity;
    this->AddPrimitive(new Sphere(material, vec3(0,0,0), scale));
}

void Bullet::Animate(float deltaTime) {
    velocity.z -= 1 * deltaTime;
    pos = pos + velocity * deltaTime;
    //printf("Bullet pos: %f %f %f\n", pos.x, pos.y, pos.z);
    for (size_t i = 0; i < Scene::Get()->TankCount(); i++)
    {
        Tank* t = Scene::Get()->GetTank(i);
        if (t == source) continue;
        if (length(t->pos+vec3(0,0,0.9) - pos) < 0.8f) {
            t->Destroy();
            return;
        }
    }
    
}
