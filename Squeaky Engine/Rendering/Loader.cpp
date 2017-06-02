#include "Loader.h"

int Loader::CreateVAO()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	VAOList.push_back(VAO);
	glBindVertexArray(VAO);
	return VAO;
}

void Loader::StoreDataInAttributeList(int attribNum, GLint size,std::vector<float> data)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	VBOList.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),&data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribNum);
	glVertexAttribPointer(attribNum, size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::UnbindVAO()
{
	glBindVertexArray(0);

}

void Loader::BindIndicesBuffer(std::vector<GLuint> indices)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	VBOList.push_back(VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint),
		&indices[0], GL_STATIC_DRAW);
}

RawModel Loader::LoadToVAO(std::vector<GLfloat> vertices, std::vector<GLuint> indices,std::vector<GLfloat>texCoords)
{
	GLuint VAO = CreateVAO();
	BindIndicesBuffer(indices);
	StoreDataInAttributeList(0,3, vertices);
	StoreDataInAttributeList(1,2, texCoords);
	UnbindVAO();
	return RawModel(VAO, indices.size());
}

RawModel Loader::LoadToVAO(MeshData data)
{
	GLuint VAO = CreateVAO();
	BindIndicesBuffer(data.indices);
	StoreDataInAttributeList(0, 3, data.vertices);
	StoreDataInAttributeList(1, 2, data.texCoords);
	StoreDataInAttributeList(2, 3, data.normals);
	UnbindVAO();
	return RawModel(VAO, data.indices.size());
}

void Loader::CleanUp()
{
	for (GLuint vao : VAOList)
	{
		glDeleteVertexArrays(1, &vao);
	}
	for (GLuint vbo : VBOList)
	{
		glDeleteBuffers(1, &vbo);
	}
	for (GLuint textureID : texturesIDList)
	{
		glDeleteTextures(1, &textureID);
	}
}
#include <iostream>
GLuint Loader::TextureFromFile(std::string filename)
{
	//Generate texture ID and load texture data 

	GLuint textureID;
	glGenTextures(1, &textureID);
	texturesIDList.push_back(textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	if (image == NULL)
	{
		std::cout << "ERROR LOADING IMAGE " << filename << std::endl;
	}
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}
void Loader::End()
{
	this->CleanUp();
}
