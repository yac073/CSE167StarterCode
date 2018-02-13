#pragma once
#include "core.h"
#include "Tokenizer.h"
using namespace std;
using namespace glm;
class Key {	
public:
	enum TanRule
	{
		none,
		flat,
		linear,
		smooth,
		floatVal
	};
	float time, value, tangentIn, tangentOut;
	TanRule RuleIn, RuleOut;
	float A, B, C, D;
	Key();
	bool Load(Tokenizer * t);
};