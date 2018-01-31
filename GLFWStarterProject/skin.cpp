#include "skin.h"

bool Skin::Load(Tokenizer & t)
{
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
			for (int i = 0; i < numTriangles; i++) {
				vec3 triangle;
				triangle.x = t.GetInt();
				triangle.y = t.GetInt();
				triangle.z = t.GetInt();
				triangles.push_back(triangle);
			}
		}
		else if (strcmp(temp, "bindings") == 0) {
			numJoints = t.GetInt();
			for (int x = 0; x < numJoints; x++) {
				mat4x3 mat;
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 3; j++) {
						mat[i][j] = t.GetFloat();
					}
				}
				bindings.push_back(mat);
			}
		}
		else t.SkipLine(); // Unrecognized token
	}
}
