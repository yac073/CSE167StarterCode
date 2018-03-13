////////////////////////////////////////
// SpinningCube.cpp
////////////////////////////////////////

#include "SpinningCube.h"

////////////////////////////////////////////////////////////////////////////////

SpinningCube::SpinningCube() {
	// Initialize constant data
	Position=glm::vec3(0);
	Axis=glm::vec3(0,1,0);		// Y-Axis
	CubeModel.MakeBox(glm::vec3(-0.2f),glm::vec3(.2f));
	SpinDelta=0.01f;

	// Resets variable data
	Reset();
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Update() {
	// Update (animate) any variable properties
	//Angle+=SpinDelta;
	//WorldMtx=glm::rotate(glm::mat4(1.0f),Angle,Axis);
	//WorldMtx[3]=glm::vec4(Position,1);
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Reset() {
	// Reset dynamic variables to a default state
	Angle=0;
	WorldMtx=glm::mat4(1);		// Reset to identity matrix
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Draw(const glm::mat4 &viewProjMtx,uint shader) {
	CubeModel.Draw(WorldMtx,viewProjMtx,shader);
}


void SpinningCube::MoveLeft(bool isLeft)
{
	Move(isLeft, vec3(-1.f, .0f, .0f));
}

void SpinningCube::MoveRight(bool isLeft)
{
	Move(isLeft, vec3(1.f, .0f, .0f));
}

void SpinningCube::MoveTop(bool isLeft)
{
	Move(isLeft, vec3(.0f, 1.0f, .0f));
}

void SpinningCube::MoveDown(bool isLeft)
{
	Move(isLeft, vec3(.0f, -1.0f, .0f));
}

void SpinningCube::MoveNear(bool isLeft)
{
	Move(isLeft, vec3(0.f, .0f, 1.0f));
}

void SpinningCube::MoveFar(bool isLeft)
{
	Move(isLeft, vec3(0.f, .0f, -1.0f));
}

vec3 SpinningCube::GetPosition()
{
	return WorldMtx[3];
}

void SpinningCube::Move(bool isLeft, vec3 dir)
{
	WorldMtx = WorldMtx * translate(mat4(1.0f), dir * .1f);
}
////////////////////////////////////////////////////////////////////////////////
