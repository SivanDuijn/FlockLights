#include "Vector3.h"
#include "BoidUtils.h"
#include "Boid.h"
#include <iostream>

using namespace std;

Boid::Boid(Vector3 p, Vector3 v)
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
	float sepMultiplier) {

	Vector3 sep = Vector3(0, 0, 0);
	Vector3 ali = vel.normalized();
	Vector3 coh = pos.copy();

	float nBsInView = 0;
	for (int i = 0; i < nBoids; i++) {

		Boid &bInView = BoidUtils::correctEdgeOverflowPerceptionR(pos, boids[i], sizeBox, percR);

		if (this != &bInView && (pos - bInView.pos).length() <= percR) {
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

	sep /= nBsInView;
	sep *= maxForce;

	ali /= nBsInView + 1;
	ali = steerForceFromVector(ali, maxSpeed, maxForce);

	coh /= nBsInView + 1;
	coh = steerTowards(coh, maxSpeed, maxForce);

	/*Boid *bsInView = new Boid[nBoids - 1];
	int nBsInView = calcBoidsInView(boids, nBoids, bsInView, percR, sizeBox);
	Vector3 s = calcSeparation(bsInView, nBsInView, percR, maxForce);
	Vector3 a = calcAlignment(bsInView, nBsInView, maxSpeed, maxForce);
	Vector3 c = calcCohesion(bsInView, nBsInView, maxSpeed, maxForce);

	s *= sepMultiplier;
	c *= .4;
	a *= .2;*/
	//sep.log();
	//ali.log();
	//coh.log();
	//std::cout << nBsInView << endl;
	flockForceToApply = sep + coh + ali;
}

void Boid::update(float maxSpeed, float maxForce, float secondsPast, Vector3 &boxSize) {

	flockForceToApply.limit(maxForce);
	flockForceToApply *= secondsPast;
	vel += flockForceToApply;

	vel.limit(maxSpeed);

	pos += vel * secondsPast;

	pos.x = BoidUtils::correctEdgeAxis(pos.x, 0, boxSize.x);
	pos.y = BoidUtils::correctEdgeAxis(pos.y, 0, boxSize.y);
	pos.z = BoidUtils::correctEdgeAxis(pos.z, 0, boxSize.z);
}

//// fill bsInView array and return the number of boids in view
//int Boid::calcBoidsInView(Boid *boids, int nBoids, Boid *bsInView, float percR, Vector3 sizeBox) {
//  int nBsInView = 0;
//  for (int i = 0; i < nBoids; i++) {
//    Boid boid = BoidUtils::correctEdgeOverflowPerceptionR(pos, boids[i], sizeBox, percR);
//
//    if (this != &boid && (pos - boid.pos).length() <= percR) {
//      bsInView[nBsInView] = boid;
//      nBsInView++;
//    }
//  }
//  return nBsInView;
//}
//
//Vector3 Boid::calcSeparation(Boid* bsInView, int nBsInView, float percR, float maxForce) {
//  if (nBsInView == 0) return Vector3(0, 0, 0);
//
//  Vector3 sep = Vector3(0, 0, 0);
//
//  Vector3 pushForce;
//  for (int i = 0; i < nBsInView; i++) {
//    pushForce = pos - bsInView[i].pos;
//    float l = pushForce.length();
//    pushForce.normalize();
//    pushForce *= 1 - (l / percR);
//    sep += pushForce;
//  }
//
//  sep /= nBsInView;
//  sep *= maxForce;
//  return sep;
//}
//Vector3 Boid::calcAlignment(Boid* bsInView, int nBsInView, float maxSpeed, float maxForce) {
//  if (nBsInView == 0) return Vector3(0, 0, 0);
//
//  Vector3 averageHeading = vel.normalized();
//
//  for (int i = 0; i < nBsInView; i++) 
//    if (bsInView[i].vel.length() > 0)
//      averageHeading += bsInView[i].vel.normalized();
//
//  averageHeading /= nBsInView + 1;
//  return steerForceFromVector(averageHeading, maxSpeed, maxForce);
//}
//Vector3 Boid::calcCohesion(Boid* bsInView, int nBsInView, float maxSpeed, float maxForce) {
//  if (nBsInView == 0) return Vector3(0, 0, 0);
//
//  Vector3 centerOfMass = Vector3(0, 0, 0);
//  centerOfMass += pos;
//
//  for (int i = 0; i < nBsInView; i++) 
//    centerOfMass += bsInView[i].pos;
//
//  centerOfMass /= nBsInView + 1;
//  return steerTowards(centerOfMass, maxSpeed, maxForce);
//}

Vector3& Boid::steerTowards(Vector3 &dest, float maxSpeed, float maxForce) {
	if (pos == dest) { Vector3 v = Vector3(0, 0, 0); return v; }

	dest -= pos;
	return steerForceFromVector(dest, maxSpeed, maxForce);
}
Vector3& Boid::steerForceFromVector(Vector3 &desired, float maxSpeed, float maxForce) {
	desired.setMag(maxSpeed);
	desired -= vel;
	desired *= (maxForce / maxSpeed);
	return desired;
}
