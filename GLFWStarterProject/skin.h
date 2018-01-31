#pragma once
#include "../core.h"
#include "../Tokenizer.h"
#include <vector>
using namespace std;
using namespace glm;
struct JointWeight {
	int joint;
	float weight;
};
class Skin {
private:
	int numVerts, numTriangles, numJoints;
	vector<vec3> positions, normals;
	vector<vector<JointWeight>> skinWeights;
	vector<vec3> triangles;
	vector<mat4x3> bindings;
public:
	Skin();
	bool Load(Tokenizer& t);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
};