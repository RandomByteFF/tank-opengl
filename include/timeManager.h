#pragma once
#include "framework.h"

class Time {
    private:
        static float lastFrame;
        static float deltaTime;
    public:
        static void Update();
        static float DeltaTime();
};
