#ifndef UTILS_H
#define UTILS_H

#include "Boid.h"
#include "Vector3.h"
#include "Utils.h"

namespace Utils {

	void init();

	// 0 - max, max including
	int randInt(int max);
	float randFloat(float max);
	Vector3 randVecInBox(Vector3 box);

	float easeIn(float n, float p = 3);
	float easeOut(float n, float p = 3);
	float easeInOut(float n);

	float map(float v, float cMin, float cMax, float nMin, float nMax);
}

#endif