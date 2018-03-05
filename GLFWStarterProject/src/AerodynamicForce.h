#pragma once
#include "core.h"
#include "Particle.h"
using namespace glm;
class AerodynamicForce {
	float p = 1.225f;
	float Cd = .5f;
	Particle* P1, *P2, *P3;
public:
	AerodynamicForce(Particle* p1, Particle *p2, Particle *p3);
	void ComputeForce(vec3 Va);
};