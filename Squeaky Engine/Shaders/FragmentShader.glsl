#version 330 core

out vec4 vFragColor;	//fragment shader output

in vec2 vUV;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

// lights

// diffuse
uniform sampler2D textureSampler;
uniform vec3 lightColor;

// specular
uniform float shineDamper;
uniform float reflectivity;
void main()
{

	//difuse
	vec3 unitNormal=normalize(surfaceNormal);
	vec3 unitLightVector=normalize(toLightVector);
	float nDotl=dot(unitLightVector,unitNormal);
	float brightness=max(nDotl,0.2);
	vec3 diffuse = brightness*lightColor;


	//specular 
	vec3 unitVectorToCamera=normalize(toCameraVector);
	vec3 lightDirection= -unitLightVector;
	vec3 reflectedLightDirection=reflect(lightDirection,unitNormal);
	float specularFactor=dot(reflectedLightDirection,unitVectorToCamera);
	specularFactor=max(specularFactor,0.0);
	float dampedFactor=pow(specularFactor,shineDamper);
	vec3 finalSpecular=dampedFactor*reflectivity* lightColor;

	vFragColor = vec4(diffuse,1.0)*texture(textureSampler,vUV)+vec4(finalSpecular,1.0);
}