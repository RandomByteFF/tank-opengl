#pragma once
#include "gameObject.h"
#include "paramSurface.h"
#include "tank.h"
//#include "scene.h"

class Tank;
//class Scene;

//TODO: killz
class Bullet : public GameObject {
    vec3 velocity;
    public:
    const Tank* source;
    Bullet(Material *material, vec3 pos, vec3 scale, vec3 velocity, Tank* source);
    void Animate(float deltaTime) override;
};

class Sphere : public ParamSurface {
public:
    Sphere(Material* material, vec3 pos, vec3 scale) : ParamSurface(material) {
        this->pos = pos;
        this->scale = scale;
        this->Create(20, 20);
    }

    VertexData GenVertexData(float u, float v) {
        VertexData vd;
        vd.tex = vec2(u,v);
        vd.pos = vec3(cos(u*2*M_PI)*sin(v*M_PI)/2, sin(u*2*M_PI)*sin(v*M_PI)/2, cos(v*M_PI)/2);
        vd.norm = normalize(vd.pos);
        return vd;
    }
};