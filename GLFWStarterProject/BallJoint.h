#pragma once
#include "Joint.h"
#include "../Model.h"
class BallJoint : public Joint {
protected:
	glm::vec3 Offset, Pose, BoxMin, BoxMax;	
	Model * model;
public:
	BallJoint(): Joint(){
		localTransform = glm::mat4(1.0f);
		worldTransform = glm::mat4(1.0f);
		dofX = Dof(-100000, 100000);
		dofY = Dof(-100000, 100000);
		dofZ = Dof(-100000, 100000);
		Offset = glm::vec3(0.0f);
		Offset = glm::vec3(0.0f);
		BoxMin = glm::vec3(-0.1f);
		BoxMax = glm::vec3(0.1f);
		Pose = glm::vec3(0.0f);
		model = new Model();
	};
	~BallJoint();
	void Update();
	bool Load(Tokenizer &t);
	void AddChild(Joint* j);
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void SetChildrenWorldTransform(glm::mat4);
};