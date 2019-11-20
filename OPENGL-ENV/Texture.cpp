#include "Texture.h"
#include <iostream>



Texture::Texture(std::string data)
{
	m_data = data;

	Load();
}


Texture::~Texture()
{
}

bool Texture::Load()
{
	SDL_Surface* imageSDL = IMG_Load(m_data.c_str());

	if (imageSDL == 0)
	{
		std::cerr << "Error : " << SDL_GetError() << std::endl;
		return false;
	}

	GLenum formatInterne = 0;
	GLenum format = 0;

	if (imageSDL->format->BytesPerPixel == 3)
	{
		formatInterne = GL_RGB;

		if (imageSDL->format->Rmask == 0xff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}

	else if (imageSDL->format->BytesPerPixel == 4)
	{
		formatInterne = GL_RGBA;

		if (imageSDL->format->Rmask == 0xff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	}

	else
	{
		std::cerr << "Error, unresolved internal format" << std::endl;
		SDL_FreeSurface(imageSDL);

		return false;
	}

	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, imageSDL->w, imageSDL->h, 0, format, GL_UNSIGNED_BYTE, imageSDL->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(imageSDL);

	return true;
}

GLuint Texture::getTextureID()
{
	return m_texture;
}
