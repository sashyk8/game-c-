// game.h

#ifndef GAME_H
#define GAME_H

void SimulatePlayer(float* p, float* dp, float ddp, float dt);
bool AabbVsAabb(float p1x, float p1y, float hs1x, float hs1y,
    float p2x, float p2y, float hs2x, float hs2y);

#endif // GAME_H
