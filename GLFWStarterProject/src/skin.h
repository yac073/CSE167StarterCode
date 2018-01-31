#pragma once
#include "core.h"
#include "Tokenizer.h"
#include "Skeleton.h"
#include "SkinDrawer.h"
#include "bmpLoader.h"
#include <vector>
using namespace std;
using namespace glm;
struct JointWeight {
	int joint;
	float weight;
};

struct MorphPosition {
	int index;
	vec3 position;
	vec3 normal;
};

class Skin {
private:
	int numVerts, numTriangles, numJoints;
	vector<vec3> positions, normals, triangles;
	vector<int>  m1, m2;
	vector<MorphPosition> morph[2];
	vector<vec2> texcoords;
	vector<string> material;
	vector<vector<JointWeight>> skinWeights;	
	vector<mat4x3> bindings;
	Skeleton * skel;
	SkinDrawer * drawer;
	BMPImage * texture;
	bool hasTexture, hasMorph;
	int tick1, tick2, m1trans, m2trans;
public:
	Skin(Skeleton*, bool hasMorph = false);
	~Skin();
	vector<ModelVertex> vtx;
	vector<uint> itx;
	bool Load(const char *file);
	bool LoadMorph(int, const char* file);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void startm1();
	void startm2();
};