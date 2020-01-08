#include "Utils.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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

	float map(float v, float start1, float stop1, float start2, float stop2) {
		return ((v-start1)/(stop1-start1))*(stop2-start2)+start2;
	}
}