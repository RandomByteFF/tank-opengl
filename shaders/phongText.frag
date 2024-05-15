#version 330
precision highp float;

uniform vec3 ks, ka;
uniform float shine;
uniform vec3 La, Le;

uniform sampler2D text;

in vec2 uv;
in vec3 wNormal;
in vec3 wView;
in vec3 wLight;
out vec4 outColor;

void main() {
    vec3 N = normalize(wNormal);
    vec3 V = normalize(wView);
    vec3 L = normalize(wLight);
    vec3 H = normalize(L + V);
    float cost = max(dot(N, L), 0);
    float cosd = max(dot(N, H), 0);
    vec3 color = ka * La + (texture(text, uv).xyz * cost + ks * pow(cosd, shine)) * Le;
    outColor = vec4(color, 1);
}