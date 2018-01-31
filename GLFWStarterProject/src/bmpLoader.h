#pragma once

#include "core.h"
// storage for one texture


class BMPImage
{
public:
	GLuint texture[1];
	~BMPImage()
	{
		delete[] data;
	}
	bool load(char* filename);
	void LoadGLTextures(char* filename);

	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};