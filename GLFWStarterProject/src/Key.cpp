#include "Key.h"

Key::Key()
{
	RuleIn = RuleOut = none;
	tangentIn = tangentOut = A = B = C = D =.0f;
}

bool Key::Load(Tokenizer * t)
{	
	char temp[256];
	time = t->GetFloat();
	value = t->GetFloat();

	t->SkipWhitespace();
	char c = t->CheckChar();
	if (c == '-' || isdigit(c)) {
		tangentIn = t->GetFloat();
	}
	else {
		t->GetToken(temp);
		if (strcmp(temp, "flat") == 0) {
			RuleIn = flat;
		} 
		else if (strcmp(temp, "linear") == 0) {
			RuleIn = linear;
		}
		else if (strcmp(temp, "smooth") == 0) {
			RuleIn = smooth;
		}
		else {
			return false;
		}
	}

	t->SkipWhitespace();
	c = t->CheckChar();
	if (c == '-' || isdigit(c)) {
		tangentOut = t->GetFloat();
	}
	else {
		t->GetToken(temp);
		if (strcmp(temp, "flat") == 0) {
			RuleOut = flat;
		}
		else if (strcmp(temp, "linear") == 0) {
			RuleOut = linear;
		}
		else if (strcmp(temp, "smooth") == 0) {
			RuleOut = smooth;
		}
		else {
			return false;
		}
	}
	return true;
}
