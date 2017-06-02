#include "ModelMesh.h"

MeshData ModelMesh::LoadModel(string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
	}
	return ProcessNode(scene->mRootNode, scene);
}
MeshData ModelMesh::ProcessNode(aiNode * node, const aiScene * scene)
{
		aiMesh* mesh = scene->mMeshes[0];
		return (ProcessMesh(mesh, scene));
}

MeshData ModelMesh::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	MeshData data;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		
		//positions
		data.vertices.push_back(mesh->mVertices[i].x);
		data.vertices.push_back(mesh->mVertices[i].y);
		data.vertices.push_back(mesh->mVertices[i].z);
		// Normals
		data.normals.push_back(mesh->mNormals[i].x);
		data.normals.push_back(mesh->mNormals[i].y);
		data.normals.push_back(mesh->mNormals[i].z);
		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			data.texCoords.push_back(mesh->mTextureCoords[0][i].x);
			data.texCoords.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			data.texCoords.push_back(0);
			data.texCoords.push_back(0);
		}
	}
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			data.indices.push_back(face.mIndices[j]);
	}
	
	return data;
}
