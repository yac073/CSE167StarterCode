#include "Curve.h"
using namespace std;
Curve::Curve() {
}

void Curve::refreshPoints(Channel* channel, float offset)
{
	auto pts = channel->points;
	auto minPT = channel->minPT;
	auto maxPT = channel->maxPT;
	auto ptDis = 0.25f / (maxPT - minPT);
	auto ptAvg = (maxPT + minPT) / 2.0f;
	for (int i = 0; i < 800; i++) {
		float t = i;		
		vertices.push_back(vec3(t / 1600.0f + 0.4f, (pts[i] - ptAvg) * ptDis + offset, 0.0f));
	}
	auto tanPT = channel->tanPT;
	extraInfo = 0;
	for each (auto info in tanPT) {
		auto t = info.x;
		auto ts = info.w;
		if (ts != 0) {
			while (t >= -4) { t -= ts; }
			t += ts;
			float dx = 0.00625f;
			float y1 = (info.y - info.z - ptAvg) * ptDis + offset;
			float y2 = (info.y + info.z - ptAvg) * ptDis + offset;
			float avgY = (y1 + y2) / 2.0f;
			y2 = (y2 - avgY) * 0.05f + avgY;
			y1 = (y1 - avgY) * 0.05f + avgY;
			while (t < 4) {
				float x = 0.4f + (t + 4.0f) / 16.0f;				
				vertices.push_back(vec3(x - dx, y1, 0.0f));
				vertices.push_back(vec3(x + dx, y2, 0.0f));
				t += ts;
				extraInfo += 1;
			}
		}
	}
	//vertices.push_back(vec3(0.0f));
	//vertices.push_back(vec3(.1f));
	//vertices.push_back(vec3(0.0f, .1f, 0.0f));
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		sizeof(glm::vec3), // 3 * sizeof(GLfloat) Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Curve::~Curve()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Curve::draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
	glLineWidth(2.0f);
	glBindVertexArray(VAO);
	glUniform1i(glGetUniformLocation(shaderProgram, "isTan"), 0);
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size() - extraInfo * 2);	
	glUniform1i(glGetUniformLocation(shaderProgram, "isTan"), 1);

	for (int i = 0; i < extraInfo; i++) {
		glDrawArrays(GL_LINE_STRIP, vertices.size() - extraInfo * 2 + i * 2
			, 2);
	}
	glBindVertexArray(0);
}

void Curve::refreshCurve(Channel* channel, float offset)
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	vertices.clear();
	refreshPoints(channel, offset);
}

