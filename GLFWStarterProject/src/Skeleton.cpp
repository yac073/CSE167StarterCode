#include "Skeleton.h"

Skeleton::Skeleton()
{
}

bool Skeleton::Load(const char * file)
{
	index = 0;
	Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");
	// Parse tree
	base = new BallJoint();	
	activeJoint = base;
	joints.push_back(base);
	base->Load(token, &joints); 
	// Finish
	token.Close();
	return true;
}

void Skeleton::Update()
{
	base->Update();
}

void Skeleton::Draw(const glm::mat4 &viewProjMtx, uint shader)
{
	base->Draw(viewProjMtx, shader);
}

void Skeleton::NextJoint()
{
	index++;
	index %= joints.size();
	activeJoint = (BallJoint*) joints[index];
}

void Skeleton::LastJoint()
{
	index += joints.size() - 1;
	index %= joints.size();
	activeJoint = (BallJoint*)joints[index];
}

void Skeleton::ModJoint(float x, float y, float z)
{
	activeJoint->ChangePose(x, y, z);
}


