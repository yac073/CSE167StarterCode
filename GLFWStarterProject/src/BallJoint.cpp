#include "BallJoint.h"

BallJoint::~BallJoint()
{
	for each(auto j in children) {
		delete(j);
	}
	delete(model);
}

void BallJoint::Update()
{
	localTransform = glm::translate(glm::mat4(1.0f), Offset) *
		glm::rotate(glm::mat4(1.0f), dofZ.getValue(Pose.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::rotate(glm::mat4(1.0f), dofY.getValue(Pose.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), dofX.getValue(Pose.x), glm::vec3(1.0f, 0.0f, 0.0f));	
	for each(auto child in children) {
		child->SetChildrenWorldTransform(worldTransform * localTransform);
		child->Update();
	}
}

bool BallJoint::Load(Tokenizer & t, std::vector<Joint*> * joints)
{
	t.FindToken("{");
	while (1) {
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "offset") == 0) {
			Offset.x = t.GetFloat();
			Offset.y = t.GetFloat();
			Offset.z = t.GetFloat();
		}
		else if (strcmp(temp, "boxmin") == 0) {
			BoxMin.x = t.GetFloat();
			BoxMin.y = t.GetFloat();
			BoxMin.z = t.GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0) {
			BoxMax.x = t.GetFloat();
			BoxMax.y = t.GetFloat();
			BoxMax.z = t.GetFloat();
		}
		else if (strcmp(temp, "rotxlimit") == 0) {
			float min = t.GetFloat();
			float max = t.GetFloat();
			dofX = Dof(min, max);
		}
		else if (strcmp(temp, "rotylimit") == 0) {
			float min = t.GetFloat();
			float max = t.GetFloat();
			dofY = Dof(min, max);
		}
		else if (strcmp(temp, "rotzlimit") == 0) {
			float min = t.GetFloat();
			float max = t.GetFloat();
			dofZ = Dof(min, max);
		}
		else if (strcmp(temp, "pose") == 0) {
			Pose.x = t.GetFloat();
			Pose.y = t.GetFloat();
			Pose.z = t.GetFloat();
		}
		else if (strcmp(temp, "balljoint") == 0) {
			BallJoint *j = new BallJoint();
			joints->push_back(j);
			j->Load(t, joints);
			AddChild(j);
		}
		else if (strcmp(temp, "}") == 0) { 
			model->MakeBox(BoxMin, BoxMax);
			return true;
		}
		else t.SkipLine(); // Unrecognized token
	}
}

void BallJoint::AddChild(Joint * j)
{
	children.push_back(j);
	j->SetParent(this);
}

void BallJoint::Draw(const glm::mat4 &viewProjMtx, uint shader)
{	
	model->Draw(worldTransform * localTransform, viewProjMtx, shader);
	for each(auto child in children) {
		child->Draw(viewProjMtx, shader);
	}
}

void BallJoint::SetChildrenWorldTransform(glm::mat4 m)
{
	worldTransform = glm::mat4(m);
}

void BallJoint::SetParent(Joint * p)
{
	parent = p;
}

void BallJoint::ChangePose(float x, float y, float z)
{
	Pose.x += x;
	Pose.y += y;
	Pose.z += z;
}
