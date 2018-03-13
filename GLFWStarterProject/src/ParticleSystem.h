#pragma once
#include "Particle.h"
#include "core.h"
#include "SpringDamper.h"
#include "AerodynamicForce.h"
#include "RopeSystem.h"
#include <vector>
using namespace glm;
using namespace std;
class ParticleSystem {
	int NumParticles;
	vector<Particle*> P;	
	vector<SpringDamper*> Sd;
	vector<AerodynamicForce*> Af;
	vector<ModelVertex> Vertices;	
	vector<RopeSystem *> Ropes;
	vector<uint> Indices;
	uint VertexBuffer, IndexBuffer;
	void Move(bool, vec3);
public:
	bool isEC;
	ParticleSystem(int);
	~ParticleSystem();
	void AddParticle(Particle *p);
	void BuildStructure();
	void Update(float deltaTime, vec3);
	void Draw(bool, const glm::mat4 &viewProjMtx, uint shader, uint);
	void MoveLeft(bool);
	void MoveRight(bool);
	void MoveTop(bool);
	void MoveDown(bool);
	void MoveNear(bool);
	void MoveFar(bool);
};