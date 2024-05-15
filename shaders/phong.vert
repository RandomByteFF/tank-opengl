#version 330
precision highp float;

uniform mat4 M, Minv, MVP;
uniform vec3 wEye;
uniform vec3 lDir;

layout(location = 0) in vec3 vtxPos; // pos in model sp
layout(location = 1) in vec3 vtxNorm;// normal in model sp
layout(location = 2) in vec2 textPos;

out vec3 wNormal;
out vec3 wView;
out vec3 wLight;

void main() {
    gl_Position = vec4(vtxPos, 1) * MVP; // to NDC
    vec4 wPos = vec4(vtxPos, 1) * M;
    wLight = vec3(1, 2, 3);
    wView = wEye - wPos.xyz/wPos.w;
    wNormal = (Minv * vec4(vtxNorm, 0)).xyz;
}