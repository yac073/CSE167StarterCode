#pragma once
#include "core.h"
#include "Tokenizer.h"
#include <vector>
#include "Channel.h"
using namespace std;
using namespace glm;
struct JointChannel {
	Channel* xChannel;
	Channel* yChannel;
	Channel* zChannel;
};
class Animation {
private:
	float startTime, endTime;
	int numOfChannels;
	vector<JointChannel*> channels;
	mat4 cubicMat;
public:
	vector<vec3> Result;
	Animation();
	~Animation();
	bool Load(const char*file);
	void Evaluate(float time);
	JointChannel* getChannels(int);
};