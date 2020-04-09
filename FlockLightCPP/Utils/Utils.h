#ifndef UTILS_H
#define UTILS_H

#include "Vector3.h"

using namespace std;

namespace Utils {

	void init();

	// 0 - max, max including
	int randInt(int max);
	float randFloat(float max);
	Vector3 randVec(float min, float max);
	Vector3 randVecInBox(Vector3 box, float extraSpaceOnSides);

	float easeIn(float n, float p = 3);
	float easeOut(float n, float p = 3);
	float easeInOut(float n);

	float map(float v, float cMin, float cMax, float nMin, float nMax);
}

#endif