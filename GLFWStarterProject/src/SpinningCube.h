////////////////////////////////////////
// SpinningCube.h
////////////////////////////////////////

#pragma once

#include "Model.h"
using namespace glm;

////////////////////////////////////////////////////////////////////////////////

// SpinningCube is an example of a basic animating object. It can be used as a
// pattern for creating more complex objects.

class SpinningCube {
public:
	SpinningCube();

	void Update();
	void Reset();
	void Draw(const glm::mat4 &viewProjMtx,uint shader);
	void MoveLeft(bool);
	void MoveRight(bool);
	void MoveTop(bool);
	void MoveDown(bool);
	void MoveNear(bool);
	void MoveFar(bool);
	vec3 GetPosition();

private:
	// Constants
	Model CubeModel;
	glm::vec3 Position;
	glm::vec3 Axis;
	float SpinDelta;

	// Variables
	float Angle;
	glm::mat4x4 WorldMtx;
	void Move(bool, vec3);
};

////////////////////////////////////////////////////////////////////////////////
