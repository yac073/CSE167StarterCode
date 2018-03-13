#pragma once
#include "core.h"
#include "Particle.h"
using namespace glm;
class SpringDamper {
	float RestLength;
	Particle *P1, *P2;
public:
	float SpringConstant, DampingFactor;
	SpringDamper(Particle*, Particle*, float rl = 0.5f);
	void ComputeForce();
};
