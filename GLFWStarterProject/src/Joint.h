#pragma once
#include "core.h"
#include "dof.h"
#include "Tokenizer.h"
#include <vector>
using namespace std;
class Joint {
public:
	glm::mat4 localTransform, worldTransform;
	Dof dofX, dofY, dofZ;
	Joint * parent;
	vector<Joint *> children;
	virtual void Update() = 0;
	virtual bool Load(Tokenizer &, vector<Joint*> *) = 0;
	virtual void AddChild(Joint *) = 0;
	virtual void Draw(const glm::mat4 &viewProjMtx, uint shader) = 0;
	virtual void SetChildrenWorldTransform(glm::mat4) = 0;
	virtual void SetParent(Joint*) = 0;
};