#ifndef FLOCK_H
#define FLOCK_H

#include "Boid.h"
#include "Vector3.h"

class Flock {
    public:
    Boid *boids;
    Vector3 boxSize, destination;
    float maxSpeed, maxForce, percR;
    int amount;
    //bool newDest = false;
    //ulong newDestTimer;

    Flock() { };
    Flock(int amount, Vector3 boxSize, float maxSpeed, float maxForce, float percR);

    void updatePos(float secondsPassed);
    void calcFlockForces();

    void updateForcesAndPos(float secondsPassed);
};

#endif