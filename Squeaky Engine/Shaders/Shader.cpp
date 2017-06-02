
#include "Shader.h"

Shader::Shader(void)
{
	
	_totalShaders = 0;
	_shaders[VERTEX_SHADER] = 0;
	_shaders[FRAGMENT_SHADER] = 0;
	_shaders[GEOMETRY_SHADER] = 0;
	_attributeList.clear();
	_uniformLocationList.clear();
			
}

Shader::~Shader(void)
{
	_attributeList.clear();
	_uniformLocationList.clear();
}

void Shader::DeleteShaderProgram() {
	glDeleteProgram(programID);
}

GLuint Shader::GetProgarmID()
{
	return this->programID;
}
void Shader::LoadFromString(GLenum type, const string& source) {
	GLuint shader = glCreateShader(type);

	const char * ptmp = source.c_str();
	glShaderSource(shader, 1, &ptmp, NULL);

	//check whether the shader loads fine
	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		cout << "Compile log: " << infoLog << endl;
		delete[] infoLog;
	}

	_shaders[_totalShaders++] = shader;
}

void Shader::CreateAndLinkProgram() {
	programID = glCreateProgram();
	if (_shaders[VERTEX_SHADER] != 0) {
		glAttachShader(programID, _shaders[VERTEX_SHADER]);
	}
	if (_shaders[FRAGMENT_SHADER] != 0) {
		glAttachShader(programID, _shaders[FRAGMENT_SHADER]);
	}
	if (_shaders[GEOMETRY_SHADER] != 0) {
		glAttachShader(programID, _shaders[GEOMETRY_SHADER]);
	}

	//link and check whether the program links fine
	GLint status;
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, NULL, infoLog);
		cerr << "Link log: " << infoLog << endl;
		delete[] infoLog;
	}

	glDeleteShader(_shaders[VERTEX_SHADER]);
	glDeleteShader(_shaders[FRAGMENT_SHADER]);
	glDeleteShader(_shaders[GEOMETRY_SHADER]);
}

void Shader::Use() {
	glUseProgram(programID);
}

void Shader::UnUse() {
	glUseProgram(0);
}

void Shader::AddAttribute(const string& attribute) {
	_attributeList[attribute] = glGetAttribLocation(programID, attribute.c_str());

}

//An indexer that returns the location of the attribute
GLuint Shader::operator [](const string& attribute) {
	return _attributeList[attribute];
}

void Shader::AddUniform(const string& uniform) {
	_uniformLocationList[uniform] = glGetUniformLocation(programID, uniform.c_str());
}

void Shader::AddUniforms(const std::vector<string> uniforms)
{
	for (string uniform : uniforms)
	{
		this->AddUniform(uniform);
	}
}

void Shader::SetUniform(GLuint uniformLocation, const mat4 & m)
{
	if (uniformLocation >= 0)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE,glm::value_ptr(m));
	}
	else
	{
		cout << "Location is wrong" << endl;
	}
}

void Shader::SetUniform(GLuint uniformLocation, const float & f)
{
	if (uniformLocation >= 0)
	{
		glUniform1f(uniformLocation, f);
	}
	else
	{
		cout << "Location is wrong" << endl;
	}
}

void Shader::SetUniform(GLuint uniformLocation, const vec3 & v)
{
	this->SetUniform(uniformLocation, v.x, v.y, v.z);
}

void Shader::SetUniform(GLuint uniformLocation, GLfloat x, GLfloat y, GLfloat z)
{
	if (uniformLocation >= 0)
	{
		glUniform3f(uniformLocation, x, y, z);
	}
	else
	{
		cout << "Location is wrong" << endl;
	}
}

void Shader::SetPointLightUniforms(PointLight light, int index)
{
	string idx = to_string(index);
	this->SetUniform(this->operator()("pointLights[" + idx + "].position"), light.position);
	this->SetUniform(this->operator()("pointLights[" + idx + "].ambient"), light.ambient);
	this->SetUniform(this->operator()("pointLights[" + idx + "].diffuse"), light.diffuse);
	this->SetUniform(this->operator()("pointLights[" + idx + "].specular"), light.specular);
	this->SetUniform(this->operator()("pointLights[" + idx + "].constant"), light.constant);
	this->SetUniform(this->operator()("pointLights[" + idx + "].linear"), light.linear);
	this->SetUniform(this->operator()("pointLights[" + idx + "].quadratic"), light.quadratic);
}

void Shader::SetDirectionalLightUniforms(DirectionalLight light)
{
	this->SetUniform(this->operator()("dirLight.direction"), light.direction);
	this->SetUniform(this->operator()("dirLight.ambient"  ), light.ambient);
	this->SetUniform(this->operator()("dirLight.diffuse"  ), light.diffuse);
	this->SetUniform(this->operator()("dirLight.specular" ), light.specular);
	
}

void Shader::SetSpotLightUniforms(SpotLight light)
{
	this->SetUniform(this->operator()("spotLight.position"), light.position);
	this->SetUniform(this->operator()("spotLight.direction"), light.direction);
	this->SetUniform(this->operator()("spotLight.ambient"), light.ambient);
	this->SetUniform(this->operator()("spotLight.diffuse"), light.diffuse);
	this->SetUniform(this->operator()("spotLight.specular"), light.specular);
	this->SetUniform(this->operator()("spotLight.constant"), light.constant);
	this->SetUniform(this->operator()("spotLight.linear"), light.linear);
	this->SetUniform(this->operator()("spotLight.quadratic"), light.quadratic);
	this->SetUniform(this->operator()("spotLight.cutOff"), light.cutOff);
	this->SetUniform(this->operator()("spotLight.outerCutOff"), light.outerCutOff);
}

GLuint Shader::operator()(const string& uniform) {
	return _uniformLocationList[uniform];
}

void Shader::End()
{
	this->DeleteShaderProgram();
}

#include <fstream>
void Shader::LoadFromFile(GLenum whichShader, const string& filename) {
	std::string ShaderCode;
	std::ifstream ShaderStream(filename, std::ios::in);
	if (ShaderStream.is_open()) {
		std::string Line = "";
		while (getline(ShaderStream, Line))
			ShaderCode += "\n" + Line;
		ShaderStream.close();
		LoadFromString(whichShader, ShaderCode);
	}
	else {
		std::cout << filename << std::endl;
		printf("Impossible to open %s. Are you in the right directory ? !\n", filename);
		getchar();
	}
}
