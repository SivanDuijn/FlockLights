#ifndef FLOCK_H
#define FLOCK_H

#include "Boid.h"
#include "../Utils/Vector3.h"
#include <chrono>

#define FLOCK_MAX_WAIT_DEST_REACHED 10  // seconds

class Flock {
    public:
    Boid *boids;
    Vector3 boxSize, destination;
    float maxSpeed, maxForce, percR;
    int amount;

    // variables for waiting random secs after reaching the destination
    bool destReached = false;
    std::chrono::_V2::steady_clock::time_point destReachedTimestamp;
    int waitSeconds;

    //bool easingSepMult = false;
    //float easingProgress = 0; // value from 0 to 1
    //float easingProgressIncreaseUnit = 0.01;
    float sepMult = 3;
    float sepMultTarget = 5;
    float sepMultUnitIncreaseUnit = .001;
    //float oldSepMult = 3;
    //float sepMultDiff = 2; // difference between target and current

    Flock() { };
    Flock(int amount, Vector3 boxSize, float maxSpeed, float maxForce, float percR);

    void updatePos(float secondsPassed);
    void calcFlockForces();
    void updateNewDest();
    void updateSepMultiplier(float secondsPassed);

    void updateEverything(float secondsPassed);
};

#endif