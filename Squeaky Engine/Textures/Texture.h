#pragma once

#include <GL\glew.h>
class Texture
{
public:
	Texture();
	Texture(GLuint  textureID);
	GLuint GetTextureID() const;
	void SetShineDamper(GLfloat shineDamper);
	void SetReflectivity(GLfloat reflectivity);
	GLfloat GetShineDamper();
	GLfloat GetReflectivity();

private:
	GLfloat shineDamper;
	GLfloat reflectivity;
	GLuint textureID;
};