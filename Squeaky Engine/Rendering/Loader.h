#pragma once
#include "..\Models\RawModel.h"
#include <iostream>
#include <vector>
#include <string>
#include <soil.h>
#include "ModelMesh.h"
class Loader
{
private:
	std::vector<GLuint> VAOList;
	std::vector<GLuint> VBOList;
	std::vector<GLuint> texturesIDList;
	int CreateVAO();
	void StoreDataInAttributeList(int attribNum,GLint size, std::vector<float> data);
	void UnbindVAO();
	void BindIndicesBuffer(std::vector<GLuint> indices);
	void CleanUp();
public:
	RawModel LoadToVAO(std::vector<GLfloat> vertices,std::vector<GLuint> indices, std::vector<GLfloat>texCoords);
	RawModel LoadToVAO(MeshData data);
	GLuint TextureFromFile(std::string filename);
	void End();
};