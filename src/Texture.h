#pragma once
#include <GL/glew.h>
#include <iostream>
#include "stb_image.h"

enum ImageType {
	PNG,
	JPG
};


class Texture
{
private:
	GLenum textureUnit;
	unsigned int texture;
	ImageType imageType;
	int width, height, nrChannels;

	
public:
	Texture( const char* filePath, ImageType imageType);
	void bindTexture(GLenum textureUnit);

private:
	void loadImage(unsigned char* data);
};

