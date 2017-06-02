#pragma once
#include <GL/glew.h>
class RawModel {
public:
	RawModel(GLuint VAO, GLsizei vertexCount);
	RawModel();
	GLuint GetVAO() const;
	GLsizei GetVertexCount() const;
private:
	GLuint VAO, VBO, EBO;
	GLsizei vertexCount;
};