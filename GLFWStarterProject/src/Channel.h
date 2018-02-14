#pragma once
#include "core.h"
#include "Tokenizer.h"
#include <vector>
#include "Key.h"
using namespace std;
using namespace glm;
class Channel {
	enum Extrapolate {
		constant,
		linear,
		cycle,
		cycle_offset,
		bounce
	};
private:
	Extrapolate extrapIn, extrapOut;
	int numKeys;
	Key** keys;
	Key* nextKey(int);
	Key* lastKey(int);
	int bSearch(float, int, int);
	mat4 * cubicMat;
	float timeSpan;
	float inverseTimeSpan;
	float bigDeltaV;
public:
	Channel(mat4 *);
	~Channel();
	float* points;
	vector<vec4> tanPT;
	float minPT, maxPT;
	bool Load(Tokenizer * t);
	void PreCompute();
	float Evaluate(float time);
};