#include "enemyAI.h"

float EnemyAI::actTimer = 3.f;
Tank* EnemyAI::enemy = nullptr;
Tank* EnemyAI::player = nullptr;

void EnemyAI::Update() {
    if (enemy && player) {
        if (enemy->destroyed) return;
        if (actTimer <= 0) {
            actTimer = 3.f;

            vec3 p = player->pos - enemy->pos;
            p.z = 0;
            enemy->RotateTurret(atan2(p.y, p.x) - atan2(enemy->GetLookDirection().y, enemy->GetLookDirection().x));

            enemy->Shoot();
        }
        else {
            actTimer -= Time::DeltaTime();
        }
    }
}
