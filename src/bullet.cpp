#include "bullet.h"

Bullet::Bullet(Material *material, vec3 pos, vec3 scale, vec3 velocity, Tank* source) : GameObject(pos) {
    this->source = source;
    this->velocity = velocity;
    this->AddPrimitive(new Sphere(material, vec3(0,0,0), scale));
}

void Bullet::Animate(float deltaTime) {
    velocity.z -= 1 * deltaTime;
    pos = pos + velocity * deltaTime;

    for (size_t i = 0; i < Scene::Get()->TankCount(); i++)
    {
        Tank* t = Scene::Get()->GetTank(i);
        if (t == source) continue;
        vec3 tankPos = t->pos +vec3(0,0,0.55);
        vec3 rel_pos = pos-tankPos;
        vec3 rel_velocity = velocity;
        float s = dot(rel_pos, rel_velocity);
        s = s > 1.0f ? 1.0f : s;
        s = s < 0.f ? 0.f : s;
        vec3 closest = rel_pos + rel_velocity * s + tankPos;
        if (length(closest - tankPos) < 0.8f) {
            t->Destroy();
            return;
        }
    }
    
}
