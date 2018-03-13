#include "RopeSystem.h"

RopeSystem::RopeSystem()
{
	P = vector<Particle*>();
	Vertices = vector<ModelVertex>();
	Sd = vector<SpringDamper*>();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

RopeSystem::~RopeSystem()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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

void RopeSystem::AddRope(Particle * p, vec3 dir)
{
	baseParticle = p;
	for (int i = 1; i < 20; i++) {
		float j = i;
		Particle* pp = new Particle((i == 19? 50.f : 10.f), p->Position + dir * (j / 190.f));
		P.push_back(pp);
	}
}

void RopeSystem::BuildStructure()
{
	auto sd = new SpringDamper(baseParticle, P[0]);
	Sd.push_back(sd);
	sd->SpringConstant *= 10.f;
	for (int i = 0; i < P.size() - 1; i++) {
		auto sd = new SpringDamper(P[i], P[i + 1]);
		Sd.push_back(sd);
		sd->SpringConstant *= 10.f;
	}
	Vertices.push_back({ vec3(baseParticle->Position) });
	for (int i = 0; i < P.size(); i++) {
		Vertices.push_back({ vec3(P[i]->Position) });
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(ModelVertex), Vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(ModelVertex), // 3 * sizeof(GLfloat) Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RopeSystem::Update(float deltaTime, vec3 windDir)
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	Vertices.clear();
	// Compute forces
	vec3 gravity(0, -9.8, 0);
	for (int i = 0; i < P.size(); i++) {		
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
	Vertices.push_back({ vec3(baseParticle->Position) });
	for (int i = 0; i < P.size(); i++){
		P[i]->Update(deltaTime); 
		Vertices.push_back({ vec3(P[i]->Position) });		
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(ModelVertex), Vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(ModelVertex), // 3 * sizeof(GLfloat) Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RopeSystem::Draw(bool debug, const glm::mat4 & viewProjMtx, uint shader)
{
	/*if (debug) {
		for each(auto p in P) {
			p->Draw(viewProjMtx, shader);
		}
	}*/
	mat4 modelMtx(1.0f);

	glUseProgram(shader);
	glLineWidth(2.0f);
	glBindVertexArray(VAO);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMtx"), 1, false, (float*)&modelMtx);

	glm::mat4 mvpMtx = viewProjMtx * modelMtx;
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewProjMtx"), 1, false, (float*)&mvpMtx);

	// Set up state
	glDrawArrays(GL_LINE_STRIP, 0, Vertices.size());

	// Clean up state

	glBindVertexArray(0);

	glUseProgram(0);
}

Particle * RopeSystem::GetLastParticle()
{
	return P[P.size() - 1];
}
