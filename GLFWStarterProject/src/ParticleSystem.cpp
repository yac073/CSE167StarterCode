#include "ParticleSystem.h"

void ParticleSystem::Move(bool isLeft, vec3 dir)
{
	int ppr = sqrt(NumParticles);
	for (int i = 0; i < ppr; i++) {
		float j = i;
		if (isLeft) {
			P[i * ppr]->Position += (dir * .1f * (1 - j / (ppr - 1)));
		}
		else {
			P[i * ppr]->Position += (dir * .1f * (j / (ppr - 1)));
		}
	}
}

ParticleSystem::ParticleSystem(int i)
{
	isEC = true;
	NumParticles = i;
	P = vector<Particle*>();
	Vertices = vector<ModelVertex>();
	Sd = vector<SpringDamper*>();
	Af = vector<AerodynamicForce*>();
	Ropes = vector<RopeSystem*>();
	Indices = vector<uint>();
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &IndexBuffer);	
}

ParticleSystem::~ParticleSystem()
{
	glDeleteBuffers(1, &IndexBuffer);
	glDeleteBuffers(1, &VertexBuffer);
	for each(auto p in P) {
		delete(p);
	}
	for each(auto p in Sd) {
		delete(p);
	}
	for each(auto p in Af) {
		delete(p);
	}
	for each(auto p in Ropes) {
		delete(p);
	}
}

void ParticleSystem::AddParticle(Particle * p)
{
	P.push_back(p);
}

void ParticleSystem::BuildStructure()
{
	int ppr = sqrt(NumParticles);
	if (!isEC) {
		for (int i = 0; i < ppr; i++) {
			P[i * ppr]->SetFix(true);
		}
	}
	for (int i = 0; i < ppr; i++) {
		for (int j = 0; j < ppr; j++) {
			if (i != ppr - 1) {
				auto sd = new SpringDamper(P[i * ppr + j], P[(i + 1) * ppr + j], 0.5f);
				Sd.push_back(sd);
			}
			if (j != ppr - 1) {
				auto sd = new SpringDamper(P[i * ppr + j], P[i * ppr + j + 1], 0.5f);
				Sd.push_back(sd);
			}
			if (i != ppr - 1 && j != ppr - 1) {
				auto sd = new SpringDamper(P[i * ppr + j], P[(i + 1) * ppr + j + 1], .707f);
				Sd.push_back(sd);
			}
			if (i != ppr - 1 && j != 0) {
				auto sd = new SpringDamper(P[i * ppr + j], P[(i + 1) * ppr + j - 1], .707f);
				Sd.push_back(sd);
			}
		}
	}
	for (int i = 0; i < ppr - 1; i++) {
		for (int j = 0; j < ppr - 1; j++) {
			auto af = new AerodynamicForce(P[(i + 1) * ppr + j], P[i * ppr + j],
				P[i * ppr + j + 1]);
			Af.push_back(af);
			Indices.push_back((i + 1) * ppr + j);
			Indices.push_back(i * ppr + j);
			Indices.push_back(i * ppr + j + 1);
			af = new AerodynamicForce( P[i * ppr + j + 1],P[(i + 1) * ppr + j + 1],
				P[(i + 1) * ppr + j]);
			Af.push_back(af);			
			Indices.push_back(i * ppr + j + 1); 
			Indices.push_back((i + 1) * ppr + j + 1);
			Indices.push_back((i + 1) * ppr + j);
		}
	}
	// Store index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(uint), &Indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	for (int i = 0; i < NumParticles; i++) {
		Vertices.push_back({ vec3(P[i]->Position), vec3(P[i]->Normal) });
	}

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(ModelVertex), &Vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (isEC) {
		auto rope = new RopeSystem();
		rope->AddRope(P[P.size() - 1], vec3(-110.f, -100.f, -110.f));
		Ropes.push_back(rope);
		rope->BuildStructure();
		auto p0 = rope->GetLastParticle();
		rope = new RopeSystem();
		rope->AddRope(P[0], vec3(110.f, -100.f, 110.f));
		Ropes.push_back(rope);
		rope->BuildStructure();
		auto p1 = rope->GetLastParticle();
		rope = new RopeSystem();
		rope->AddRope(P[ppr - 1], vec3(110.f, -100.f, -110.f));
		Ropes.push_back(rope);
		rope->BuildStructure();
		auto p2 = rope->GetLastParticle();
		rope = new RopeSystem();
		rope->AddRope(P[P.size() - ppr], vec3(-110.f, -100.f, 110.f));
		Ropes.push_back(rope);
		rope->BuildStructure();
		auto p3 = rope->GetLastParticle();
		auto sd = new SpringDamper(p0, p1, 0.5f);
		sd->SpringConstant *= 100.f;
		Sd.push_back(sd);
		sd = new SpringDamper(p1, p2, 0.5f);
		sd->SpringConstant *= 100.f;
		Sd.push_back(sd);
		sd = new SpringDamper(p2, p3, 0.5f);
		sd->SpringConstant *= 100.f;
		Sd.push_back(sd);
		sd = new SpringDamper(p3, p0, 0.5f);
		sd->SpringConstant *= 100.f;
		Sd.push_back(sd);
	}
}

void ParticleSystem::Update(float deltaTime, vec3 windDir)
{
	glDeleteBuffers(1, &VertexBuffer);
	glGenBuffers(1, &VertexBuffer);
	int ppr = sqrt(NumParticles);
	Vertices = vector<ModelVertex>();	
	// Compute forces
	vec3 gravity(0, -9.8, 0);
	for (int i = 0; i < NumParticles; i++) {		
		vec3 force = gravity * (P[i]->Mass); // f=mg
		P[i]->ApplyForce(force);
	}

	for each(auto sd in Sd) {
		sd->ComputeForce();
	}
	for each(auto af in Af) {
		af->ComputeForce(windDir);
	}
	for each (auto rope in Ropes)
	{
		rope->Update(deltaTime, windDir);
	}
	// Integrate
	for (int i = 0; i < NumParticles; i++){
		P[i]->Update(deltaTime); 
		Vertices.push_back({ vec3(P[i]->Position), vec3(P[i]->Normal) });		
	}

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(ModelVertex), &Vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::Draw(bool debug, const glm::mat4 & viewProjMtx, uint shader, uint ropeShader)
{
	/*if (debug) {
		for each(auto p in P) {
			p->Draw(viewProjMtx, shader);
		}
	}*/
	mat4 modelMtx(1.0f);

	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMtx"), 1, false, (float*)&modelMtx);

	glm::mat4 mvpMtx = viewProjMtx * modelMtx;
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewProjMtx"), 1, false, (float*)&mvpMtx);

	// Set up state
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	uint posLoc = 0;
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);

	uint normLoc = 1;
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)12);

	// Draw geometry
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Clean up state
	glDisableVertexAttribArray(normLoc);
	glDisableVertexAttribArray(posLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
	for each(auto rope in Ropes) {
		rope->Draw(true, viewProjMtx, ropeShader);
	}
}

void ParticleSystem::MoveLeft(bool isLeft)
{
	Move(isLeft, vec3(-1.f, .0f, .0f));
}

void ParticleSystem::MoveRight(bool isLeft)
{
	Move(isLeft, vec3(1.f, .0f, .0f));
}

void ParticleSystem::MoveTop(bool isLeft)
{
	Move(isLeft, vec3(.0f, 1.0f, .0f));
}

void ParticleSystem::MoveDown(bool isLeft)
{
	Move(isLeft, vec3(.0f, -1.0f, .0f));
}

void ParticleSystem::MoveNear(bool isLeft)
{
	Move(isLeft, vec3(0.f, .0f, 1.0f));
}

void ParticleSystem::MoveFar(bool isLeft)
{
	Move(isLeft, vec3(0.f, .0f, -1.0f));
}

