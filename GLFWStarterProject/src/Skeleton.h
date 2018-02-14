#pragma once
#include "core.h"
#include <iostream>
#include "BallJoint.h"
#include "Animation.h"
#include "Curve.h"
class Skeleton {
private:
	BallJoint* base;	
	int index;
	Curve * xC, *yC, *zC;
public:
	std::vector<Joint *> joints;
	Animation* anim;
	BallJoint* activeJoint;
	Skeleton();
	~Skeleton();
	bool Load(const char *file);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void DrawCurve(const glm::mat4 &viewProjMtx, uint shader);
	void NextJoint();
	void refreshCurve();
	void LastJoint();
	void ModJoint(float x, float y, float z);
};