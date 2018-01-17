#include "dof.h"

Dof::Dof()
{
	min = max = 0;
}

Dof::Dof(float minn, float maxx)
{
	min = minn; max = maxx;
}

float Dof::getValue(float x)
{
	return glm::clamp(x, min, max);
}

