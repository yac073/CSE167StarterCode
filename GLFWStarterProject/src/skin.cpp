#define _CRT_SECURE_NO_WARNINGS

#include "skin.h"
Skin::Skin(Skeleton* s, bool hasMorph)
{
	skel = s;
	drawer = new SkinDrawer();
	tick1 = 0;
	tick2 = 0;
	this->hasMorph = hasMorph;
}

Skin::~Skin()
{
	delete(drawer);
	delete(texture);
}

bool Skin::Load(const char *file)
{
	Tokenizer t;
	t.Open(file);
	while (1) {
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "positions") == 0) {
			numVerts = t.GetInt();
			t.FindToken("{");
			for (int i = 0; i < numVerts; i++) {
				vec3 position;
				position.x = t.GetFloat();
				position.y = t.GetFloat();
				position.z = t.GetFloat();
				positions.push_back(position);
				m1.push_back(-1);
				m2.push_back(-1);
			}
		}
		else if (strcmp(temp, "normals") == 0) {
			numVerts = t.GetInt();
			t.FindToken("{");
			for (int i = 0; i < numVerts; i++) {
				vec3 normal;
				normal.x = t.GetFloat();
				normal.y = t.GetFloat();
				normal.z = t.GetFloat();
				normals.push_back(normal);
			}
		}
		else if (strcmp(temp, "skinweights") == 0) {
			numVerts = t.GetInt();
			t.FindToken("{");
			for (int i = 0; i < numVerts; i++) {
				vector<JointWeight> jointWeights;
				int numAttachment = t.GetInt();
				for (int j = 0; j < numAttachment; j++) {
					JointWeight jointWeight;
					jointWeight.joint = t.GetInt();
					jointWeight.weight = t.GetFloat();
					jointWeights.push_back(jointWeight);
				}
				skinWeights.push_back(jointWeights);
			}
		}
		else if (strcmp(temp, "triangles") == 0) {
			numTriangles = t.GetInt();
			t.FindToken("{");
			for (int i = 0; i < numTriangles; i++) {
				vec3 triangle;
				triangle.x = t.GetInt();
				triangle.y = t.GetInt();
				triangle.z = t.GetInt();
				triangles.push_back(triangle);
			}
		}
		else if (strcmp(temp, "texcoords") == 0) {
			numVerts = t.GetInt();
			t.FindToken("{");
			for (int i = 0; i < numVerts; i++) {
				vec2 coord;
				coord.x = t.GetFloat();
				coord.y = t.GetFloat();
				texcoords.push_back(coord);
			}
		}
		else if (strcmp(temp, "material") == 0) {			
			t.FindToken("texture ");
			char c = t.GetChar();
			string s;
			while (c != '\n') {
				s += c;
				c = t.GetChar();
			}
			material.push_back(s);
			int n = s.length();
			// declaring character array
			char char_array[100];
			// copying the contents of the 
			// string to char array
			strcpy(char_array, s.c_str());
			texture = new BMPImage();
			texture->LoadGLTextures(char_array);
			hasTexture = true;
		}
		else if (strcmp(temp, "bindings") == 0) {
			numJoints = t.GetInt();
			t.FindToken("{");
			for (int x = 0; x < numJoints; x++) {
				mat4x3 mat;
				t.FindToken("matrix");
				t.FindToken("{");
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 3; j++) {
						mat[i][j] = t.GetFloat();
					}
				}
				bindings.push_back(mat);
			}
			t.Close();
			//itx part
			for (int i = 0; i < numTriangles; i++) {
				for (int j = 0; j < 3; j++) {
					itx.push_back(triangles[i][j]);
				}				
			}
			return true;
		}
		else t.SkipLine(); // Unrecognized token
	}
}

bool Skin::LoadMorph(int n, const char * file)
{
	Tokenizer t;
	t.Open(file);
	while (1) {
		char temp[256];
		t.GetToken(temp);
		if (strcmp(temp, "positions") == 0) {
			int numVerts = t.GetInt();
			t.FindToken("{");
			for (int i = 0; i < numVerts; i++) {
				MorphPosition position;
				position.index = t.GetInt();
				position.position.x = t.GetFloat() - positions[position.index].x;
				position.position.y = t.GetFloat() - positions[position.index].y;
				position.position.z = t.GetFloat() - positions[position.index].z;
				morph[n].push_back(position);
				if (n == 0) {
					m1[position.index] = i;
				}
				else {
					m2[position.index] = i;
				}
			}
		}
		else if (strcmp(temp, "normals") == 0) {
			int numVerts = t.GetInt();
			t.FindToken("{");
			for (int i = 0; i < numVerts; i++) {
				t.GetInt();
				morph[n][i].normal.x = t.GetFloat() - normals[morph[n][i].index].x;
				morph[n][i].normal.y = t.GetFloat() - normals[morph[n][i].index].y;
				morph[n][i].normal.z = t.GetFloat() - normals[morph[n][i].index].z;
			}
			t.Close();
			return true;
		}		
		else t.SkipLine(); // Unrecognized token
	}
}

void Skin::Update()
{
	vtx = vector<ModelVertex>();
	for (int i = 0; i < numVerts; i++) {
		int attachmentNum = skinWeights[i].size();
		vec3 vPrime(0.0f);		
		vec3 nPrime(0.0f);
		vec3 pos = vec3(positions[i]);
		vec3 nom = vec3(normals[i]);
		if (hasMorph) {
			if (m1[i] != -1) {
				pos += morph[0][m1[i]].position * (tick1 / 60.0f);
				nom += morph[0][m1[i]].normal * (tick1 / 60.0f);
			}
			if (m2[i] != -1) {
				pos += morph[1][m2[i]].position * (tick2 / 60.0f);
				nom += morph[1][m2[i]].normal * (tick2 / 60.0f);
			}
		}
		for (int j = 0; j < attachmentNum; j++) {
			int jointID = skinWeights[i][j].joint;
			vec3 vDelta = skinWeights[i][j].weight * (skel->joints[jointID]->worldTransform * skel->joints[jointID]->localTransform) * inverse(mat4(bindings[jointID])) * vec4(pos, 1.0f);
			vec3 nDelta = skinWeights[i][j].weight * (skel->joints[jointID]->worldTransform * skel->joints[jointID]->localTransform) * inverse(mat4(bindings[jointID])) * vec4(nom, 0.0f);
			vPrime += vDelta;
			nPrime += nDelta;

		}
		vtx.push_back({ vPrime, normalize(nPrime),hasTexture? texcoords[i] : vec2(0.0f) });
	}
	if (hasMorph) {
		tick1 += m1trans; if (tick1 == 0 || tick1 == 60) { m1trans = 0; }
		tick2 += m2trans; if (tick2 == 0 || tick2 == 60) { m2trans = 0; }
	}
}

void Skin::Draw(const glm::mat4 & viewProjMtx, uint shader)
{
	if (vtx.size() == 0) { return; }
	drawer->SetBuffers(vtx, itx);	
	drawer->Draw(mat4(1.0f), viewProjMtx, shader, hasTexture, hasTexture? texture->texture[0] : 0);
	
}

void Skin::startm1()
{
	if (m1trans == 0 && tick1 == 0) {
		m1trans = 1;
	} else if (m1trans == 0 && tick1 == 60) {
		m1trans = -1;
	} else {
		m1trans = -1 * m1trans;
	}
}

void Skin::startm2()
{
	if (m2trans == 0 && tick2 == 0) {
		m2trans = 1;
	}
	else if (m2trans == 0 && tick2 == 60) {
		m2trans = -1;
	}
	else {
		m2trans = -1 * m2trans;
	}
}
