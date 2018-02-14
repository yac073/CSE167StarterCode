#include "Animation.h"

Animation::Animation()
{
	cubicMat = mat4(0.0f);
	cubicMat[0][0] = 2.0f;	cubicMat[1][0] = -2.0f; cubicMat[2][0] = 1.0f;	cubicMat[3][0] = 1.0f;
	cubicMat[0][1] = -3.0f; cubicMat[1][1] = 3.0f;	cubicMat[2][1] = -2.0f; cubicMat[3][1] = -1.0f;
	cubicMat[0][2] = .0f;	cubicMat[1][2] = .0f;	cubicMat[2][2] = 1.0f;	cubicMat[3][2] = .0f;
	cubicMat[0][3] = 1.0f;	cubicMat[1][3] = .0f;	cubicMat[2][3] = .0f;	cubicMat[3][3] = .0f;
}

Animation::~Animation()
{
	for each(auto channel in channels) {
		delete(channel->xChannel);
		delete(channel->yChannel);
		delete(channel->zChannel);
		delete(channel);
	}
}

bool Animation::Load(const char * file)
{
	Tokenizer t;
	t.Open(file);
	t.FindToken("{");
	while (1) {
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "range") == 0) {
			startTime = t.GetFloat();
			endTime = t.GetFloat();
		}
		else if (strcmp(temp, "numchannels") == 0) {
			numOfChannels = t.GetInt();
		}
		else if (strcmp(temp, "channel") == 0) {
			auto jc = new JointChannel();	
			//xyz Channel
			{
				auto c = new Channel(&cubicMat);
				if (!(c->Load(&t))) {
					t.Close();
					return false;
				}
				jc->xChannel = c;
				c->PreCompute();

				c = new Channel(&cubicMat);
				if (!(c->Load(&t))) {
					t.Close();
					return false;
				}
				jc->yChannel = c;
				c->PreCompute();

				c = new Channel(&cubicMat);
				if (!(c->Load(&t))) {
					t.Close();
					return false;
				}
				jc->zChannel = c;
				c->PreCompute();
			}
			channels.push_back(jc);
			if (channels.size() == numOfChannels / 3) {
				t.Close();
				return true;
			}
		}
		else t.SkipLine(); // Unrecognized token
	}
}

void Animation::Evaluate(float time)
{
	Result.clear();
	for (int i = 0; i < numOfChannels / 3; i++) {
		float xval = channels[i]->xChannel->Evaluate(time);
		float yval = channels[i]->yChannel->Evaluate(time);
		float zval = channels[i]->zChannel->Evaluate(time);
		Result.push_back(vec3(xval,yval,zval));
	}
}

JointChannel * Animation::getChannels(int index)
{
	return channels[index];
}
