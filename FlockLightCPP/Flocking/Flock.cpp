#include "Flock.h"
#include "BoidUtils.h"
#include "../Utils/Utils.h"

Flock::Flock(int n, Vector3 bs, float mSpeed, float mForce, float percRadius) {
    amount = n;
    boxSize = bs;
    maxSpeed = mSpeed;
    maxForce = mForce;
    percR = percRadius;
    
    boids = new Boid[amount];

    for (int i = 0; i < amount; i++)
        boids[i] = Boid(
            Vector3(
                boxSize.x/2 + Utils::randFloat(3),
                boxSize.y/2 + Utils::randFloat(3),
                boxSize.z/2 + Utils::randFloat(3)
            ), Utils::randVec(-1, 1));
    
    destination = Vector3(
			Utils::randInt(boxSize.x),
			Utils::randInt(boxSize.y),
			Utils::randInt(boxSize.z)
		);
}

void Flock::calcFlockForces() {
    for (int i = 0; i < amount; i++) 
        boids[i].calcFlockForce(boids, amount, percR, maxSpeed, maxForce, boxSize, destination, sepMult);
}

void Flock::updatePos(float secondsPassed) {
    for (int i = 0; i < amount; i++) {
        boids[i].update(maxSpeed, maxForce, secondsPassed, boxSize);
        
        // check if the boids have reached the destination
        if (!destReached && (boids[i].pos - destination).length() < 50) {
            destReached = true;
            destReachedTimestamp = std::chrono::steady_clock::now();
            waitSeconds = Utils::randInt(FLOCK_MAX_WAIT_DEST_REACHED);
            //destination = Utils::randVecInBox(boxSize);
        }
    }
}

void Flock::updateNewDest() {
    if (destReached) {
        auto now = std::chrono::steady_clock::now();
        int seconds = std::chrono::duration_cast<std::chrono::seconds>(now - destReachedTimestamp).count();
        if (seconds > waitSeconds) {
            destReached = false;
            destination = Utils::randVecInBox(boxSize);
        }
    }
}

void Flock::updateSepMultiplier(float secondsPassed) {

    sepMult += sepMultUnitIncreaseUnit * secondsPassed;
    
    if ((sepMultUnitIncreaseUnit > 0 && sepMult > sepMultTarget) ||
        sepMultUnitIncreaseUnit < 0 && sepMult < sepMultTarget) 
    {
        sepMult = sepMultTarget;
        sepMultTarget = .5 + Utils::randFloat(3);
        sepMultUnitIncreaseUnit = fabsf32(sepMultUnitIncreaseUnit); // set to positive
        if (sepMult > sepMultTarget)
            sepMultUnitIncreaseUnit *= -1; // set negative
    }

    // float ease = Utils::easeInOut(easingProgress);
    // sepMult = oldSepMult + ease * sepMultDiff;

    // easingProgress += easingProgressIncreaseUnit * secondsPassed;
    // if (easingProgress > 1) {
    //     // reset
    //     float sepMultTarget = 2 + Utils::randFloat(3);
    //     sepMultDiff = sepMultTarget - sepMult;
    //     // min sepMult = 2, max speMult = 5, so max diff = 3
    //     float normalIncreaseUnit = 1;

    // }
}

void Flock::updateEverything(float secondsPassed) {
    calcFlockForces();
    updatePos(secondsPassed);
    updateNewDest();
    updateSepMultiplier(secondsPassed);
}