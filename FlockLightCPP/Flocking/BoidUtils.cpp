#include "Boid.h"
#include "BoidUtils.h"
#include "../Utils/Vector3.h"
#include "../Utils/Utils.h"

float BoidUtils::getRandomRangeWithBoxInMiddle(float m, float axis) {
	float destMaxAxis = axis * m;
	float left = (destMaxAxis - axis) / 2;
	float rand = -left;// + random(destMaxAxis + 1);
	return rand;
}
float BoidUtils::correctEdgeAxis(float axisValue, float minV, float maxV) {
	if (axisValue > maxV)
		axisValue = minV;
	else if (axisValue < minV)
		axisValue = maxV;
	return axisValue;
}
Boid BoidUtils::correctEdgeOverflowPerceptionR(Vector3 &ourPos, Boid &otherBoid, Vector3 &sizeBox, float perceptionRadius) {
	// if a boid is close to an edge, than a boid on the other side of the
	// screen could still be inside the perception radius
	// because boids will get teleported to the other side of the sceen when they fly out of the screen
	// here we return a teleported copy, if a boid on the other side is in range

	float x = correctEdgeInfinityPerceptionOnAxis(ourPos.x, otherBoid.pos.x, sizeBox.x, perceptionRadius);
	float y = correctEdgeInfinityPerceptionOnAxis(ourPos.y, otherBoid.pos.y, sizeBox.y, perceptionRadius);
	float z = correctEdgeInfinityPerceptionOnAxis(ourPos.z, otherBoid.pos.z, sizeBox.z, perceptionRadius);
	Vector3 p = Vector3(x, y, z);
	if (p == otherBoid.pos)
		return otherBoid;
	else {
		Boid b = Boid(p, otherBoid.vel);
		return b;
	}
}
float BoidUtils::correctEdgeInfinityPerceptionOnAxis(float thisAxis, float toCheckAxis, float sizeBoxAxis, float perceptionRadius) {
	if (thisAxis < toCheckAxis) {
		float overflw = perceptionRadius - thisAxis;
		if (toCheckAxis > sizeBoxAxis - overflw)
			toCheckAxis -= sizeBoxAxis;
	}
	else if (sizeBoxAxis - thisAxis < perceptionRadius) {
		float overflw = perceptionRadius - (sizeBoxAxis - thisAxis);
		if (toCheckAxis < overflw)
			toCheckAxis += sizeBoxAxis;
	}
	return toCheckAxis;
}

Boid BoidUtils::createRandomBoid(Vector3 &sizeBox, float maxVel) {
	return Boid(
		Vector3(
			Utils::randInt(sizeBox.x),
			Utils::randInt(sizeBox.y),
			Utils::randInt(sizeBox.z)
		),
		Vector3(
			Utils::randFloat(maxVel),
			Utils::randFloat(maxVel),
			Utils::randFloat(maxVel)
		)
	);
}
Boid BoidUtils::createBoid(Vector3 p, Vector3 v) {
	return Boid(Vector3(p.x, p.y, p.z), Vector3(v.x, v.y, p.z));
}


