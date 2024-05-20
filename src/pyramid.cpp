#include "pyramid.h"

Pyramid::Pyramid(Material *mat, vec3 pos, vec3 scale, vec3 rot) : GameObject(pos, scale, rot){
    Triangle* t = new Triangle(mat, vec3(0, 0, 0), vec3(1, 1, 1), vec3(M_PI/3, 0, 0));
    AddPrimitive(t);
    t = new Triangle(mat, vec3(0, 0, 0), vec3(1, 1, 1), vec3(M_PI/3, 0, M_PI/2));
    AddPrimitive(t);
    t = new Triangle(mat, vec3(0, 0, 0), vec3(1, 1, 1), vec3(M_PI/3, 0, M_PI));
    AddPrimitive(t);
    t = new Triangle(mat, vec3(0, 0, 0), vec3(1, 1, 1), vec3(M_PI/3, 0, 3*M_PI/2));
    AddPrimitive(t);
}
