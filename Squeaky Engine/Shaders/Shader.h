#pragma once


#include <GL/glew.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "..\\Lighting\Lighting.h"
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

using namespace std;

class Shader
{
public:
	Shader(void);
	~Shader(void);
	void LoadFromString(GLenum whichShader, const string& source);
	void LoadFromFile(GLenum whichShader, const string& filename);
	void CreateAndLinkProgram();
	void Use();
	void UnUse();
	void AddAttribute(const string& attribute);
	void AddUniform(const string& uniform);
	void AddUniforms(const std::vector<string> uniforms);
	void SetUniform(GLuint uniformLocation, const mat4 & m);
	void SetUniform(GLuint uniformLocation, const float & f);
	void SetUniform(GLuint uniformLocation, const vec3 & v);
	void SetUniform(GLuint uniformLocation, GLfloat x,GLfloat y,GLfloat z);
	void SetPointLightUniforms(PointLight light, int index);
	void SetDirectionalLightUniforms(DirectionalLight light);
	void SetSpotLightUniforms(SpotLight light);
	//An indexer that returns the location of the attribute/uniform
	GLuint operator[](const string& attribute);
	GLuint operator()(const string& uniform);
	void End();
	GLuint GetProgarmID();
private:
	void DeleteShaderProgram();
	enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };
	GLuint	programID;
	int _totalShaders;
	GLuint _shaders[3];//0->vertexshader, 1->fragmentshader, 2->geometryshader
	map<string, GLuint> _attributeList;
	map<string, GLuint> _uniformLocationList;
};

