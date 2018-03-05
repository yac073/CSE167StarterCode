#pragma once
#include "core.h"
#include "Particle.h"
using namespace glm;
class SpringDamper {
	float SpringConstant, DampingFactor;
	float RestLength;
	Particle *P1, *P2;
public:
	SpringDamper(Particle*, Particle*, float);
	void ComputeForce();
};
