#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(GLuint textureID)
{
	this->textureID = textureID;
}

GLuint Texture::GetTextureID() const
{
	return this->textureID;
}

void Texture::SetShineDamper(GLfloat shineDamper)
{
	this->shineDamper = shineDamper;
}

void Texture::SetReflectivity(GLfloat reflectivity)
{
	this->reflectivity = reflectivity;
}

GLfloat Texture::GetShineDamper()
{
	return this->shineDamper;
}

GLfloat Texture::GetReflectivity()
{
	return this->reflectivity;
}
