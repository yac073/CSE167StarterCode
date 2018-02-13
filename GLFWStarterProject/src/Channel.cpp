#include "Channel.h"

Key* Channel::nextKey(int now)
{
	return keys[(now + 1) % numKeys];
}

Key* Channel::lastKey(int now)
{
	return keys[(now + numKeys - 1) % numKeys];
}

Key * Channel::bSearch(float time, int min, int max)
{
	int middle = (min + max) / 2;
	if (min == middle) {
		if (time == keys[max]->time) {
			return keys[max];
		}
		else {
			return keys[min];
		}
	}
	if (time > keys[middle]->time) {
		return bSearch(time, middle, max);
	}
	else {
		return bSearch(time, min, middle);
	}
}

Channel::Channel(mat4* m)
{
	cubicMat = m;
}

Channel::~Channel()
{
	for (int i = 0; i < numKeys; i++) {
		delete(keys[i]);
	}
	delete(keys);
}

bool Channel::Load(Tokenizer * t)
{
	t->FindToken("{");
	while (1) {
		char temp[256];
		t->GetToken(temp);
		if (strcmp(temp, "extrapolate") == 0) {
			auto e_in = t->GetToken(temp);
			if (strcmp(temp, "constant") == 0) {
				extrapIn = constant;
			} 
			else if (strcmp(temp, "linear") == 0) {
				extrapIn = linear;
			}
			else if(strcmp(temp, "cycle") == 0) {
				extrapIn = cycle;
			}
			else if (strcmp(temp, "cycle_offset") == 0) {
				extrapIn = cycle_offset;
			}
			else if (strcmp(temp, "bounce") == 0) {
				extrapIn = bounce;
			}
			else {
				return false;
			}
			auto e_out = t->GetToken(temp);
			if (strcmp(temp, "constant") == 0) {
				extrapOut = constant;
			}
			else if (strcmp(temp, "linear") == 0) {
				extrapOut = linear;
			}
			else if (strcmp(temp, "cycle") == 0) {
				extrapOut = cycle;
			}
			else if (strcmp(temp, "cycle_offset") == 0) {
				extrapOut = cycle_offset;
			}
			else if (strcmp(temp, "bounce") == 0) {
				extrapOut = bounce;
			}
			else {
				return false;
			}
		}
		else if (strcmp(temp, "keys") == 0) {
			numKeys = t->GetInt();
			keys = new Key*[numKeys];
			t->FindToken("{");
			for (int i = 0; i < numKeys; i++) {
				Key* key = new Key();
				if (!(key->Load(t))) {
					return false;
				}
				keys[i] = key;
			}
			return true;
		}
		else t->SkipLine(); // Unrecognized token
	}
}

void Channel::PreCompute()
{
	for (int i = 0; i < numKeys; i++) {
		auto nowKey = keys[i];
		//tangent in
		switch (nowKey->RuleIn)
		{
		case Key::flat:
			nowKey->tangentIn = 0;
			break;
		case Key::linear:
			if (i != 0) {
				nowKey->tangentIn = 
					(nowKey->value - lastKey(i)->value) /
					(nowKey->time - lastKey(i)->time);
			}
			break;
		case Key::smooth:
			if (i != 0 && i != numKeys - 1) {
				nowKey->tangentIn =
					(nextKey(i)->value - lastKey(i)->value) /
					(nextKey(i)->time - lastKey(i)->time);
			}
			else if (i == numKeys - 1) {
				nowKey->tangentIn = 
					(nowKey->value - lastKey(i)->value) /
					(nowKey->time - lastKey(i)->time);
			}
			break;
		default:
			break;
		}
		//tangent out
		switch (nowKey->RuleOut)
		{
		case Key::flat:
			nowKey->tangentOut = 0;
			break;
		case Key::linear:
			if (i != numKeys - 1) {
				nowKey->tangentOut = 
					(nextKey(i)->value - nowKey->value) /
					(nextKey(i)->time - nowKey->time);
			}
			else {
				nowKey->tangentOut = nowKey->tangentIn;
			}
			break;
		case Key::smooth:
			if (i != 0 && i != numKeys - 1) {
				nowKey->tangentOut =
					(nextKey(i)->value - lastKey(i)->value) /
					(nextKey(i)->time - lastKey(i)->time);
			}
			else if (i == numKeys - 1) {
				nowKey->tangentOut = nowKey->tangentIn;
			}
			else {
				nowKey->tangentOut =
					(nextKey(i)->value - nowKey->value) /
					(nextKey(i)->time - nowKey->time);
			}
			break;
		default:
			break;
		}		
	}
	auto key0 = keys[0];
	if (key0->RuleIn != Key::none && key0->RuleIn != Key::flat 
		&& key0->tangentIn == 0) {
		key0->tangentIn = key0->tangentOut;
	}
	for (int i = 0; i < numKeys - 1; i++) {
		auto key0 = keys[i];
		auto key1 = keys[i + 1];
		auto deltaT = key1->time - key0->time;
		vec4 temp(key0->value, key1->value, 
			key0->tangentOut * deltaT, key1->tangentIn * deltaT);
		vec4 abcd = (*cubicMat) * temp;
		key0->A = abcd[0];
		key0->B = abcd[1];
		key0->C = abcd[2];
		key0->D = abcd[3];
	}
	auto fK = keys[0]; 
	auto lK = lastKey(0);
	timeSpan = lK->time - fK->time;
	inverseTimeSpan = 1 / timeSpan;
	bigDeltaV = lK->value - fK->value;
}

float Channel::Evaluate(float time)
{
	float tan, deltaT;
	if (time < keys[0]->time) {
		switch (extrapIn) {
		case constant:
			return keys[0]->value;
			break;
		case linear:
			tan = keys[0]->tangentIn;
			deltaT = keys[0]->time - time;
			return keys[0]->value - tan * deltaT;
			break;
		case cycle:			
			return Evaluate(time + timeSpan);
			break;
		case cycle_offset:
			return Evaluate(time + timeSpan) - bigDeltaV;
			break;
		case bounce:
			if (time + timeSpan >= keys[0]->time) {
				return Evaluate(keys[0]->time * 2 - time);
			}
			else {
				return Evaluate(time + timeSpan * 2);
			}
			break;
		default:
			return 0.0f;
			break;
		}
	}


	auto lK = lastKey(0);
	if (time > lK->time) {
		switch (extrapOut) {
		case constant:
			return lK->value;
			break;
		case linear:
			tan = lK->tangentOut;
			deltaT = time - lK->time;
			return lK->value + tan * deltaT;
			break;
		case cycle:
			return Evaluate(time - timeSpan);
			break;
		case cycle_offset:
			return Evaluate(time - timeSpan) + bigDeltaV;
			break;
		case bounce:
			if (time - timeSpan <= lK->time) {
				return Evaluate(lK->time * 2 - time);
			}
			else {
				return Evaluate(time - timeSpan * 2);
			}
			break;
		default:
			return 0.0f;
			break;
		}
	}
	if (numKeys == 1) {
		return keys[0]->value;
	}
	auto key = bSearch(time, 0, numKeys - 1);
	if (time == key->time) {
		return key->value;
	}
	else {
		float u = (time - keys[0]->time) * inverseTimeSpan;
		return key->A * u * u * u + key->B * u * u + key->C * u + key->D;
	}
}
