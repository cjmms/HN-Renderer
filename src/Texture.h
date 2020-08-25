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


};

unsigned int loadCubemap(std::vector<std::string> faces);

// create a cubemap for shadow mapping
// does not need image address as input
unsigned int createDepthCubemap(unsigned int width, unsigned int height);

void createTexture(unsigned int &id, const char* filePath, ImageType imageType);

// TODO: gamma correction

void createDepthAttachment(unsigned int &id, unsigned int width, unsigned int height);

void loadImage(unsigned char* data, ImageType imageType, unsigned int width, unsigned int height);