#pragma once
#include "Particle.h"
#include "core.h"
#include "SpringDamper.h"
#include "AerodynamicForce.h"
#include <vector>
using namespace glm;
using namespace std;
class ParticleSystem {
	int NumParticles;
	vector<Particle*> P;	
	vector<SpringDamper*> Sd;
	vector<AerodynamicForce*> Af;
	vector<ModelVertex> Vertices;	
	vector<uint> Indices;
	uint VertexBuffer, IndexBuffer;

public:
	ParticleSystem(int);
	~ParticleSystem();
	void AddParticle(Particle *p);
	void BuildStructure();
	void Update(float deltaTime, vec3);
	void Draw(bool, const glm::mat4 &viewProjMtx, uint shader);
};