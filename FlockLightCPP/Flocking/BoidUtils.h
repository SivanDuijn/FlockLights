#ifndef BoidUtils_h
#define BoidUtils_h

#include "../Utils/Vector3.h"
#include "Boid.h"

class BoidUtils
{
public:
	static float getRandomRangeWithBoxInMiddle(float m, float axis);
	static float correctEdgeAxis(float axisValue, float minV, float maxV);
	static Boid correctEdgeOverflowPerceptionR(Vector3 &ourPos, Boid& otherBoid, Vector3 &sizeBox, float perceptionRadius);

	static Boid createRandomBoid(Vector3 &sizeBox, float maxVel);

	static float correctEdgeInfinityPerceptionOnAxis(
		float thisAxis,
		float toCheckAxis,
		float sizeBoxAxis,
		float perceptionRadius
	);
};

#endif