////////////////////////////////////////
// Model.h
////////////////////////////////////////

#pragma once

#include "Core.h"

////////////////////////////////////////////////////////////////////////////////

class SkinDrawer {
public:
	SkinDrawer();
	~SkinDrawer();

	void Draw(const glm::mat4 &modelMtx,const glm::mat4 &viewProjMtx,uint shader, bool hasTex = false, uint tex = 0);
	void SetBuffers(const std::vector<ModelVertex> &vtx,const std::vector<uint> &idx);

	// Access functions

private:
	uint VertexBuffer;
	uint IndexBuffer;	

	int Count;
};

////////////////////////////////////////////////////////////////////////////////
