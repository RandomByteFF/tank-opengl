#include "camera.h"

Camera* Camera::camera;

void Camera::UploadwEye() {
    for (size_t i = 0; i < Shader::getShaderCount(); i++) {
        Shader* shader = Shader::getShader(i);
        shader->Use();
        shader->setUniform(wEye, "wEye");
    }
}

Camera::Camera()
{
    fov = M_PI/4;
    asp = 1;
    wEye = vec3(0, -6.2, 1.5);
    wLookat = vec3(0,0,1.1);
    wVup = vec3(0, 0, 1);
    fp = 0.1;
    bp = 60;
    UploadwEye();
}

mat4 Camera::V() {
    vec3 w = normalize(wEye - wLookat);
    vec3 u = normalize(cross(wVup, w));
    vec3 v = cross(w, u);
    return TranslateMatrix(-wEye) * mat4 (u.x, v.x, w.x, 0,
                                            u.y, v.y, w.y, 0,
                                            u.z, v.z, w.z, 0,
                                            0,0,0,1);
}

mat4 Camera::P() {
    float sy = 1/tanf(fov/2);
    return mat4(sy/asp, 0, 0, 0,
                0, sy, 0, 0,
                0, 0, -(fp+bp)/(bp-fp), -1,
                0, 0, -2*fp*bp/(bp-fp), 0);
}
void Camera::setTarget(vec3 target, vec3 facing) {
    wLookat = target;
    facing.z = 0;
    facing = normalize(-facing)*6.2;
    wEye = vec3(target.x + facing.x, target.y + facing.y, 1.5);
    UploadwEye();
}

Camera* const Camera::GetInstance() {
    if (camera == nullptr) {
        camera = new Camera();
    }
    return camera;
}