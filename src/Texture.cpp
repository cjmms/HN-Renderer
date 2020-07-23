#include "Texture.h"


Texture::Texture(const char* filePath, ImageType imageType)
    :imageType(imageType), textureUnit(GL_TEXTURE0)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    loadImage(data);
}

Texture::Texture(int width, int height)
    :imageType(ATTACHMENT)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Texture::bindTexture(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
}


void Texture::loadImage(unsigned char* data) 
{
    if (data) {
        switch (imageType)
        {
        case JPG:
            // 2nd arg is mipmap level, 3rd arg is the format of output data, 6th arg should always be 0.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case PNG:
            // can't load PNG properly
            // TODO
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            std::cout << "Image source type Error" << std::endl;
            return;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        // free image memory
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Image source is not properly loaded." << std::endl;
    }
}



