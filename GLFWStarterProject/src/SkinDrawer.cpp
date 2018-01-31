////////////////////////////////////////
// Model.cpp
////////////////////////////////////////

#include "SkinDrawer.h"

////////////////////////////////////////////////////////////////////////////////

SkinDrawer::SkinDrawer() {
	glGenBuffers(1,&VertexBuffer);
	glGenBuffers(1,&IndexBuffer);

	Count=0;
}

////////////////////////////////////////////////////////////////////////////////

SkinDrawer::~SkinDrawer() {
	glDeleteBuffers(1,&IndexBuffer);
	glDeleteBuffers(1,&VertexBuffer);
}

////////////////////////////////////////////////////////////////////////////////

void SkinDrawer::Draw(const glm::mat4 &modelMtx,const glm::mat4 &viewProjMtx,uint shader, bool hasTex, uint tex) {
	// Set up shader
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader,"ModelMtx"),1,false,(float*)&modelMtx);

	if (hasTex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(glGetUniformLocation(shader, "hasTex"), 1);
		glUniform1i(glGetUniformLocation(shader, "tex"), 0);
	}

	glm::mat4 mvpMtx=viewProjMtx * modelMtx;
	glUniformMatrix4fv(glGetUniformLocation(shader,"ModelViewProjMtx"),1,false,(float*)&mvpMtx);

	// Set up state
	glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);

	uint posLoc=0;
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,sizeof(ModelVertex),0);

	uint normLoc=1;
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc,3,GL_FLOAT,GL_FALSE,sizeof(ModelVertex),(void*)12);

	uint texLoc = 2;
	glEnableVertexAttribArray(texLoc);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)24);

	// Draw geometry
	glDrawElements(GL_TRIANGLES,Count,GL_UNSIGNED_INT,0);

	// Clean up state
	glDisableVertexAttribArray(normLoc);
	glDisableVertexAttribArray(posLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glUseProgram(0);
}

////////////////////////////////////////////////////////////////////////////////

void SkinDrawer::SetBuffers(const std::vector<ModelVertex> &vtx,const std::vector<uint> &idx) {
	Count=(int)idx.size();

	// Store vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,vtx.size()*sizeof(ModelVertex),&vtx[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	// Store index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,idx.size()*sizeof(uint),&idx[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

////////////////////////////////////////////////////////////////////////////////
