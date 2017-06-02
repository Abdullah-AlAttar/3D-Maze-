#pragma once
#include <GL\glew.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <soil.h>
#include <vector>
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
using namespace std;
struct MeshData
{
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLfloat> texCoords;
	vector<GLuint> indices;
};
class ModelMesh
{
public:
	static MeshData LoadModel(string path);
private:
	string directory;
	static MeshData ProcessNode(aiNode* node, const aiScene* scene);
	static MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
