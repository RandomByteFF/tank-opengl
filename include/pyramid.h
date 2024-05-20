#pragma once
#include "gameObject.h"
#include "triangle.h"

class Pyramid : public GameObject {
    public:
    Pyramid(Material *mat, vec3 pos=vec3(0,0,0), vec3 scale=vec3(1,1,1), vec3 rot=vec3(0,0,0));
    
};