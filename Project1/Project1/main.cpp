#include "Boid.h"
#include "Vector3.h"

#include "utils.h"
#include <iostream>

using namespace std;

#define AMOUNT 100

int main() {
	Utils::init();

	Boid boid(Vector3(0, 0, 0), Vector3(1,2,3));
	
	Vector3 sizeBox = Vector3(100, 100, 100);
	Boid boids[AMOUNT];

	for (int i = 0; i < AMOUNT; i++) {
		boids[i] = Boid(Vector3(Utils::randInt(100), Utils::randInt(100), Utils::randInt(100)), Vector3(Utils::randfloat(3), Utils::randfloat(3), Utils::randfloat(3)));
	}
	boids[0].calcFlockForce(boids, AMOUNT, 50, 20, 4, sizeBox, 2);

	int counter = 0;
	while (true) {
		for (int i = 0; i < AMOUNT; i++) {
			boids[i].calcFlockForce(boids, AMOUNT, 30, 20, 4, sizeBox, 2);
		}

		for (int i = 0; i < AMOUNT; i++) {
			boids[i].update(20, 4, 1, sizeBox);
		}
		counter++;
		if (counter % 100 == 0)
			cout << counter << endl;
	}


	return 0;
}
