#pragma once

#include "tank.h"

class EnemyAI {
	static float actTimer;
    public:
	static Tank* enemy;
    static Tank* player;
	static void Update();
};

