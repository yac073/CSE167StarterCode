#include "SpringDamper.h"

SpringDamper::SpringDamper(Particle * p1, Particle *p2, float rl)
{
	P1 = p1;
	P2 = p2;
	SpringConstant = 5000.f;
	DampingFactor = 2.f;
	RestLength = abs(distance(p2->Position, p1->Position));
}

void SpringDamper::ComputeForce()
{
	vec3 l = P2->Position - P1->Position;
	if (abs(l.x) == 0 && abs(l.y) == 0 && abs(l.z) == 0) {
		return;
	}
	vec3 e = normalize(l);
	float sf = (-1.f * SpringConstant) *
		(RestLength - length(P2->Position - P1->Position));
	float df = (-1.f * DampingFactor) * (dot(P1->Velocity, e) - dot(P2->Velocity, e));
	vec3 f = (sf + df) * e;
	P1->ApplyForce(f);
	P2->ApplyForce(-f);
}
