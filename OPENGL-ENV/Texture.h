#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>

class Texture
{
public:
	Texture(std::string data);
	~Texture();

	bool Load();

	GLuint getTextureID();

private:
	GLuint m_texture;
	std::string m_data;

};

