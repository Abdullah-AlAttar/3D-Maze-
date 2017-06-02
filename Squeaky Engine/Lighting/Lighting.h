#pragma once


#include <glm\glm.hpp>
using glm::vec3;
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	Light(vec3 _position, vec3 _ambient, vec3 _diffuse, vec3 _specular)
		:position(_position), ambient(_ambient), diffuse(_diffuse), specular(_specular) {}
};
struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	DirectionalLight(vec3 _direction, vec3 _ambient, vec3 _diffuse, vec3 _specular)
		:direction(_direction), ambient(_ambient), diffuse(_diffuse), specular(_specular) {}
};
struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	PointLight(vec3 _position, vec3 _ambient, vec3 _diffuse, vec3 _specular,float _constant,float _linear,float _quadratic)
		:position(_position), ambient(_ambient), diffuse(_diffuse), specular(_specular),constant(_constant),linear(_linear),quadratic(_quadratic) {}
};
struct SpotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

	SpotLight(vec3 _position, vec3 _direction,vec3 _ambient, vec3 _diffuse, vec3 _specular, float _constant, float _linear, float _quadratic,float _cutOff,float _outerCutOff)
		:position(_position),direction(_direction), ambient(_ambient), diffuse(_diffuse), specular(_specular), constant(_constant), linear(_linear), quadratic(_quadratic),cutOff(_cutOff),outerCutOff(_outerCutOff) {}
};
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	Material(vec3 _ambient, vec3 _diffuse, vec3 _specular, float _shininess)
		:ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess) {}
};
