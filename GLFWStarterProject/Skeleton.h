#pragma once
#include "../core.h"
#include "BallJoint.h"
class Skeleton {
private:
	BallJoint* base;
public:
	BallJoint* activeJoint;
	Skeleton();
	bool Load(const char *file);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
};