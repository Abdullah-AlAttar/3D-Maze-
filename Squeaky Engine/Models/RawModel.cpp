#include "RawModel.h"

RawModel::RawModel(GLuint VAO, GLsizei vertexCount)
{
	this->VAO = VAO;
	this->vertexCount = vertexCount;
}

RawModel::RawModel()
{
}

GLuint RawModel::GetVAO() const
{
	return VAO;
}

GLsizei RawModel::GetVertexCount() const
{
	return vertexCount;
}
