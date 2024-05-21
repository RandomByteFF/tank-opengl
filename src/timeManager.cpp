#include "timeManager.h"

float Time::lastFrame = 0.0f;
float Time::deltaTime = 0.0f;

void Time::Update() {
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentFrame - lastFrame) / 1000;
	lastFrame = currentFrame;
}

float Time::DeltaTime() {
    return deltaTime;
}