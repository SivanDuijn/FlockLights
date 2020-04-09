#include "BoidUtils.h"
#include "Boid.h"
#include <iostream>

#define ALI_MOD 	.2
#define COH_MOD 	.4
#define SEEK_MOD 	.3

using namespace std;

Boid::Boid(Vector3 p, Vector3 v)
{
	pos = p;
	vel = v;
}
void Boid::setPosAndVec(Vector3 p, Vector3 v)
{
	pos = p;
	vel = v;
}

void Boid::calcFlockForce(
	Boid* boids,
	int nBoids,
	float percR,
	float maxSpeed,
	float maxForce,
	Vector3 &sizeBox,
	Vector3 &dest,
	float sepMultiplier) {

	Vector3 sep = Vector3(0, 0, 0);
	Vector3 ali = vel.normalized();
	Vector3 coh = pos.copy();

	float nBsInView = 0;
	for (int i = 0; i < nBoids; i++) {

		if (this != &boids[i]) {
			Boid bInView = BoidUtils::correctEdgeOverflowPerceptionR(pos, boids[i], sizeBox, percR);
			if ((pos - bInView.pos).length() <= percR) {
				nBsInView++;

				// separation
				Vector3 pushForce = pos - bInView.pos;
				float l = pushForce.length();
				pushForce.normalize();
				pushForce *= 1 - (l / percR);
				sep += pushForce;
				// alignment
				if (bInView.vel.length() > 0)
					ali += bInView.vel.normalized();

				// cohesion
				coh += bInView.pos;
			}
		}
	}
	if (nBsInView == 0) { flockForceToApply.makeTrivial(); return; }

	sep /= nBsInView;
	sep *= maxForce;

	ali /= nBsInView + 1;
	ali = steerForceFromVector(ali, maxSpeed, maxForce);

	coh /= nBsInView + 1;
	coh = steerTowards(coh, maxSpeed, maxForce);

	sep *= sepMultiplier;
	ali *= ALI_MOD;
	coh *= COH_MOD;
	//sep.log();
	//ali.log();
	//coh.log();
	//std::cout << nBsInView << endl;
	flockForceToApply = sep + coh + ali;

	// add destination steer force
	Vector3 d = dest.copy();
	Vector3 seek = steerTowards(d, maxSpeed, maxForce);
	seek *= SEEK_MOD;
	flockForceToApply += seek;
}

void Boid::update(float maxSpeed, float maxForce, float timePast, Vector3 &boxSize) {

	flockForceToApply.limit(maxForce);
	vel += flockForceToApply * timePast;

	vel.limit(maxSpeed);
	pos += vel * timePast;

	pos.x = BoidUtils::correctEdgeAxis(pos.x, 0, boxSize.x);
	pos.y = BoidUtils::correctEdgeAxis(pos.y, 0, boxSize.y);
	pos.z = BoidUtils::correctEdgeAxis(pos.z, 0, boxSize.z);
}

Vector3& Boid::steerTowards(Vector3 &dest, float maxSpeed, float maxForce) {
	if (pos == dest) { dest = Vector3(0, 0, 0); return dest; }

	dest -= pos;
	return steerForceFromVector(dest, maxSpeed, maxForce);
}
Vector3& Boid::steerForceFromVector(Vector3 &desired, float maxSpeed, float maxForce) {
	desired.setMag(maxSpeed);
	desired -= vel;
	desired *= (maxForce / maxSpeed);
	return desired;
}
