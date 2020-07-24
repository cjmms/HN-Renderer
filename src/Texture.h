#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <string>
#include "stb_image.h"

enum ImageType {
	PNG,
	JPG,
	ATTACHMENT
};


class Texture
{
private:
	GLenum textureUnit;
	unsigned int texture;
	ImageType imageType;
	int width, height, nrChannels;

	
public:
	// create normal texture
	Texture( const char* filePath, ImageType imageType);
	
	// Create FBO attachment
	Texture( int width, int height);
	void bindTexture(GLenum textureUnit);
	inline unsigned int getTextureID() const { return texture; };

private:
	void loadImage(unsigned char* data);
};

unsigned int loadCubemap(std::vector<std::string> faces);