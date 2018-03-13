#pragma once
#include "Particle.h"
#include "core.h"
#include "SpringDamper.h"
#include "AerodynamicForce.h"
#include <vector>
using namespace glm;
using namespace std;
class RopeSystem{
	vector<Particle*> P;	
	vector<SpringDamper*> Sd;
	vector<AerodynamicForce*> Af;
	vector<ModelVertex> Vertices;	
	uint VAO, VBO;
	Particle * baseParticle;
public:
	RopeSystem();
	~RopeSystem();
	void AddRope(Particle *p, vec3 dir);
	void BuildStructure();
	void Update(float deltaTime, vec3);
	void Draw(bool, const glm::mat4 &viewProjMtx, uint shader);
	Particle* GetLastParticle();
};