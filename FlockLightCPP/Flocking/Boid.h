#ifndef Boid_h
#define Boid_h

#include "../Utils/Vector3.h"

class Boid
{
public:
	Vector3 vel;
	Vector3 pos;
	Vector3 flockForceToApply;

	Boid() { };
	Boid(Vector3 p, Vector3 v);
	void setPosAndVec(Vector3 p, Vector3 v);

	void calcFlockForce(
		Boid* boids,
		int nBoids,
		float percR,
		float maxSpeed,
		float maxForce,
		Vector3& sizeBox,
		Vector3& dest,
		float sepMultiplier
	);

	void update(float maxSpeed, float maxForce, float secondsPast, Vector3& boxSize);

	//int calcBoidsInView(       // fill bsInView array and return the number of boids in view
	//  Boid* boids, 
	//  int nBoids, 
	//  Boid* bsInView, 
	//  float perceptionRadius, 
	//  Vector3 sizeBox
	//);

	//Vector3 calcSeparation(Boid* bsInView, int nBsInView, float percR, float maxForce);
	//Vector3 calcAlignment(Boid* bsInView, int nBsInView, float maxSpeed, float maxForce);
	//Vector3 calcCohesion(Boid* bsInView, int nBsInView, float maxSpeed, float maxForce);

	Vector3& steerTowards(Vector3& dest, float maxSpeed, float maxForce);
	Vector3& steerForceFromVector(Vector3& desired, float maxSpeed, float maxForce);
};

#endif