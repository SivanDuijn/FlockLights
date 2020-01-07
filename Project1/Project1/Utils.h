#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Boid.h"
#include "Vector3.h"

namespace Utils {

	void init() {
		srand(time(NULL));
	}

	// 0 - max, max including
	int randInt(int max) {
		return rand() % (max + 1);
	}

	float randfloat(float max) {
		return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max));
	}
}

#endif