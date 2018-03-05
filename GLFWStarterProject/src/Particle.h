#pragma once
#include "core.h"
#include <stdio.h>
#include <iostream>
using namespace glm;
using namespace std;
class  Particle
{
private:

	int numOfNormal;
	vec3 tempNormal;
public:	
	bool isFixed;
	vec3 Normal;
	float Mass; // Constant kg
	vec3 Position; // Evolves frame to frame
	vec3 Velocity; // Evolves frame to frame
	vec3 Force; // Reset and re-computed each frame
	Particle(float m = 0.01f, vec3 p = vec3(0.0f), vec3 v = vec3(0.0f), 
		vec3 f = vec3(0.0f));
	void Update(float deltaTime);
	void UpdateNormal(vec3 normal);
	void SetFix(bool);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void ApplyForce(vec3 &f);
};
