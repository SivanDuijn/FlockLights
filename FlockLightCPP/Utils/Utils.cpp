// #ifndef UTILS_H
// #define UTILS_H

#include "Utils.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

namespace Utils {

    void init() {
		srand(time(NULL));
	}

	// 0 - max, max including
	int randInt(int max) {
		return rand() % (max + 1);
	}
	float randFloat(float max) {
		return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max));
	}
	Vector3 randVec(float min, float max) {
		float d = max - min;
		return Vector3(
			Utils::randFloat(d) + min,
			Utils::randFloat(d) + min,
			Utils::randFloat(d) + min
		);
	}
	Vector3 randVecInBox(Vector3 box) {
		return Vector3(
			Utils::randInt(box.x),
			Utils::randInt(box.y),
			Utils::randInt(box.z)
		);
	}
	
	float map(float v, float start1, float stop1, float start2, float stop2) {
		return ((v-start1)/(stop1-start1))*(stop2-start2)+start2;
	}

	// 0 <= n <= 1
	float easeIn(float n, float p) {
		return powf32(n, p);
	}
	float easeOut(float n, float p) {
		return 1 - (easeIn(1 - n, p));
	}
	float easeInOut(float n) {
		if (n <= .5) 
			return easeIn(n * 1.587402, 3);     // (n / .5) * 0.793701
		return easeOut((n - .5) * 1.587402 + .206299, 3);
	}
}

// #endif