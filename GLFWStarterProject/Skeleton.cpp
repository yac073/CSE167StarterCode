#include "Skeleton.h"

Skeleton::Skeleton()
{
}

bool Skeleton::Load(const char * file)
{
	Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");
	// Parse tree
	base = new BallJoint();
	base->Load(token); 
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


