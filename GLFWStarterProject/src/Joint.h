#pragma once
#include "core.h"
#include "dof.h"
#include "Tokenizer.h"
#include <vector>
using namespace std;
using namespace glm;
class Joint {
public:
	glm::mat4 localTransform, worldTransform;
	char name[1024];
	Dof dofX, dofY, dofZ;
	Joint * parent;
	vector<Joint *> children;
	virtual void Update() = 0;
	virtual bool Load(Tokenizer &, vector<Joint*> *) = 0;
	virtual void AddChild(Joint *) = 0;
	virtual void Draw(const glm::mat4 &viewProjMtx, uint shader) = 0;
	virtual void SetWorldTransform(glm::mat4) = 0;
	virtual void SetParent(Joint*) = 0;
	virtual void SetPose(glm::vec3 xyz) = 0;
	virtual vec3 GetPose() = 0;
	virtual vec3 GetPosition() = 0;
	virtual vec3 GetEndPosition() = 0;
};