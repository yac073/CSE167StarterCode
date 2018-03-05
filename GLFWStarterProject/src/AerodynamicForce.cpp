#include "AerodynamicForce.h"

AerodynamicForce::AerodynamicForce(Particle * p1, Particle * p2, Particle * p3)
{
	P1 = p1;
	P2 = p2;
	P3 = p3;
}

void AerodynamicForce::ComputeForce(vec3 Va)
{
	vec3 Vs = (P1->Velocity + P2->Velocity + P3->Velocity) / 3.f;
	vec3 v = Vs - Va;
	vec3 l1 = (P2->Position - P1->Position);
	if (abs(l1.x) == 0 && abs(l1.y) == 0 && abs(l1.z) == 0) {
		return;
	}
	vec3 l2 = (P3->Position - P1->Position);
	if (abs(l2.x) == 0 && abs(l2.y) == 0 && abs(l2.z) == 0) {
		return;
	}
	vec3 n = cross(l1, l2);
	if (l1 == l2) {
		return;
	}
	n = normalize(n);
	float a0 = 0.5f * length(P2->Position - P1->Position) * 
		length(P3->Position - P1->Position);
	float a = a0 * dot(v, n) / length(v);
	vec3 aero = -0.5f * p * length(v) * length(v) * Cd * a * n;
	P1->ApplyForce(aero / 3.f);
	P1->UpdateNormal(n);
	P2->ApplyForce(aero / 3.f);
	P2->UpdateNormal(n);
	P3->ApplyForce(aero / 3.f);
	P3->UpdateNormal(n);
}
