#pragma once
#include "core.h"
#include <iostream>
#include "BallJoint.h"
#include "Animation.h"
class Skeleton {
private:
	BallJoint* base;	
	int index;
public:
	std::vector<Joint *> joints;
	Animation* anim;
	BallJoint* activeJoint;
	Skeleton();
	bool Load(const char *file);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void NextJoint();
	void LastJoint();
	void ModJoint(float x, float y, float z);
};