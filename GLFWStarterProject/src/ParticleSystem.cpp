#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int i)
{
	NumParticles = i;
	P = vector<Particle*>();
	Vertices = vector<ModelVertex>();
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
}

void ParticleSystem::AddParticle(Particle * p)
{
	P.push_back(p);
}

void ParticleSystem::BuildStructure()
{
	int ppr = sqrt(NumParticles);
	for (int i = 0; i < ppr; i ++) {
		P[i * ppr]->SetFix(true);
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
	// Integrate
	for (int i = 0; i < NumParticles; i++){
		P[i]->Update(deltaTime); 
		Vertices.push_back({ vec3(P[i]->Position), vec3(P[i]->Normal) });		
	}

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(ModelVertex), &Vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::Draw(bool debug, const glm::mat4 & viewProjMtx, uint shader)
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
}

