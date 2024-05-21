#include "bullet.h"

Bullet::Bullet(Material *material, vec3 pos, vec3 scale, vec3 velocity) : GameObject() {
    this->velocity = velocity;
    this->AddPrimitive(new Sphere(material, pos, scale));
}

void Bullet::Animate(float deltaTime) {
    velocity.z -= 1 * deltaTime;
    pos = pos + velocity * deltaTime;
}
