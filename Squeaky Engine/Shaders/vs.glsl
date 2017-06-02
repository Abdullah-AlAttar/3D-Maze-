#version 330 core
layout(location = 0) in vec3 vPosition;	//object space vertex position
layout(location = 1) in vec2 vTexCoords;	//per-vertex colour
layout(location = 2) in vec3 vNormal;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 MVP;


void main()
{
    gl_Position =MVP * vec4(vPosition, 1.0f);
    FragPos = vec3(modelMatrix * vec4(vPosition, 1.0f));
    //Normal = mat3(transpose(inverse(modelMatrix))) * vNormal; 
    Normal=vNormal;
    TexCoords=vTexCoords; 
} 