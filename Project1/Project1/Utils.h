#ifndef UTILS_H
#define UTILS_H

#include "Boid.h"
#include "Vector3.h"

namespace Utils {

	void init();

	// 0 - max, max including
	int randInt(int max);

	float randfloat(float max);

	float map(float v, float cMin, float cMax, float nMin, float nMax);
}

#endif