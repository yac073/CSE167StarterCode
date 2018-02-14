#include "Skeleton.h"

Skeleton::Skeleton()
{
	xC = new Curve;
	yC = new Curve;
	zC = new Curve;
}

Skeleton::~Skeleton()
{
	delete xC;
	delete yC;
	delete zC;
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
	auto channels = anim->getChannels(1);
	xC->refreshPoints(channels->xChannel, 0.666f);
	yC->refreshPoints(channels->yChannel, 0.333f);
	zC->refreshPoints(channels->zChannel, 0.000f);

	return true;
}

void Skeleton::Update()
{
	if (anim != nullptr) {		
		base->SetWorldTransform(glm::translate(glm::mat4(1.0f), anim->Result[0]));
		for (int i = 0; i < anim->Result.size() - 1; i++) {
			((BallJoint*)joints[i])->SetPose(anim->Result[i + 1]);
		}
		//cout << "x: " << anim->Result[2].x << " y: " << anim->Result[2].y
		//	<< " z: " << anim->Result[2].z << endl;
	}
	base->Update();
}

void Skeleton::Draw(const glm::mat4 &viewProjMtx, uint shader)
{
	base->Draw(viewProjMtx, shader);
}

void Skeleton::DrawCurve(const glm::mat4 &viewProjMtx, uint shader)
{
	yC->draw(shader);
	xC->draw(shader);
	zC->draw(shader);
}

void Skeleton::NextJoint()
{
	index++;
	index %= joints.size();
	activeJoint = (BallJoint*)joints[index];
	cout << activeJoint->name << endl;
	refreshCurve();
}

void Skeleton::refreshCurve()
{
	auto channels = anim->getChannels(index + 1);
	xC->refreshCurve(channels->xChannel, 0.666f);
	yC->refreshCurve(channels->yChannel, 0.333f);
	zC->refreshCurve(channels->zChannel, 0.000f);
}

void Skeleton::LastJoint()
{
	index += joints.size() - 1;
	index %= joints.size();
	activeJoint = (BallJoint*)joints[index];
	cout << activeJoint->name << endl;
	refreshCurve();
}

void Skeleton::ModJoint(float x, float y, float z)
{
	activeJoint->ChangePose(x, y, z);
}


