#include "Flock.h"
#include "BoidUtils.h"
#include "Utils.h"

Flock::Flock(int n, Vector3 bs, float mSpeed, float mForce, float percRadius) {
    amount = n;
    boxSize = bs;
    maxSpeed = mSpeed;
    maxForce = mForce;
    percR = percRadius;
    
    boids = new Boid[amount];

    for (int i = 0; i < amount; i++)
        boids[i] = BoidUtils::createRandomBoid(boxSize, 5);
    
    destination = Vector3(
			Utils::randInt(boxSize.x),
			Utils::randInt(boxSize.y),
			Utils::randInt(boxSize.z)
		);
    // std::cout << "hi" << std::endl;
}

void Flock::calcFlockForces() {
    for (int i = 0; i < amount; i++)
        boids[i].calcFlockForce(boids, amount, percR, maxSpeed, maxForce, boxSize, destination, 3);
}

void Flock::updatePos(float secondsPassed) {
    for (int i = 0; i < amount; i++) {
        boids[i].update(maxSpeed, maxForce, secondsPassed, boxSize);
        if ((boids[i].pos - destination).length() < 10)
            destination = Vector3(
			Utils::randInt(boxSize.x),
			Utils::randInt(boxSize.y),
			Utils::randInt(boxSize.z)
		);
    }
}

void Flock::updateForcesAndPos(float secondsPassed) {
    calcFlockForces();
    updatePos(secondsPassed);
}