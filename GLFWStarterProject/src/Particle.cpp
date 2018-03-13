#include "Particle.h"


Particle::Particle(float m, vec3 p, vec3 v, vec3 f)
{
	Mass = m;
	Position = p;
	Velocity = v;
	Force = f;
	Normal = vec3(0.0f, -1.0f, .0f);
	tempNormal = vec3(0.0f, 1.0f, .0f);
	isFixed = false;
}

void Particle::Update(float deltaTime)
{
	if (tempNormal.x != 0 || tempNormal.y != 0 || tempNormal.z != 0) {
		Normal = normalize(-tempNormal);
		tempNormal = vec3(0.0f);
	}
	if (isFixed) {
		return;
	}
	Velocity += deltaTime * (Force / Mass);
	Position += deltaTime * Velocity;
	Force = vec3(0.0f);
	if(Position.y < -5.f)
	{
		Position.y = 2 * -5.f - Position.y;
		//Position.y = -5.f;
		Velocity.y = -.1f * Velocity.y;
		Velocity.x = (1.f - .7f) * Velocity.x; // cheezy
		Velocity.z = (1.f - .7f) * Velocity.z; // cheezy
	}
}

void Particle::UpdateNormal(vec3 normal)
{
	tempNormal += normal;
}

void Particle::SetFix(bool b)
{
	isFixed = b;
}

void Particle::Draw(const glm::mat4 & viewProjMtx, uint shader)
{
	//glPushMatrix();
	//glMultMatrixf(&(viewProjMtx[0][0]));
	//glBegin(GL_POINTS);
	//vec3 rgb = glm::normalize(normal);
	//glColor3f((rgb.x + 1) / 2, (rgb.y + 1) / 2, (rgb.z + 1) / 2);
	//vec3 pos = viewProjMtx * vec4(Position, 1);
	//glVertex3f(pos.x, pos.y, pos.z);
	//glEnd();
}

void Particle::ApplyForce(vec3 & f)
{
	Force += f;
}
