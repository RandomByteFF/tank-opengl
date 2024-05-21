#include "enemyAI.h"

float EnemyAI::actTimer = 3.f;
Tank* EnemyAI::enemy = nullptr;
Tank* EnemyAI::player = nullptr;

void EnemyAI::Update() {
    if (enemy->destroyed) return;
    if (actTimer <= 0) {
        actTimer = 3.f;
        enemy->Shoot();
    }
    else {
        actTimer -= Time::DeltaTime();
    }
}
