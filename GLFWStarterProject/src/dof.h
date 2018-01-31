#pragma once
#include "core.h"
class Dof {
private:
	float min, max;
public:
	Dof();
	Dof(float, float);
	float getValue(float);	
};