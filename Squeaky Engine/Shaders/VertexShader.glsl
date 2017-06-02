#version 330 core
 
layout(location = 0) in vec3 vPosition;	//object space vertex position
layout(location = 1) in vec2 vTexCoords;	//per-vertex colour
layout(location = 2) in vec3 vNormal;


uniform vec3 lightPosition;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 MVP;

out vec2 vUV;
out vec3 surfaceNormal;
out vec3 toLightVector;

out vec3 toCameraVector;
void main()
{
   vec4 worldPosition=modelMatrix*vec4(vPosition,1.0);
   gl_Position = MVP *vec4(vPosition,1);
   vUV=vTexCoords;
   surfaceNormal=  (modelMatrix*vec4(vNormal,0.0)).xyz;
   toLightVector=lightPosition-worldPosition.xyz;

   toCameraVector = (inverse(viewMatrix)*vec4(0.0,0.0,0.0,1.0)).xyz-worldPosition.xyz;

}