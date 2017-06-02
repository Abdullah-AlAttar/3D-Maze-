#pragma once
#include <glm\glm.hpp>
#include "Utility.h"
class BoxCollider
{
public:
	BoxCollider();
	BoxCollider(glm::vec3 _mMinExtent, glm::vec3 _mMaxExtent,std::string name);
	glm::vec3 GetMinExtent();
	glm::vec3 GetMaxExtent();
	void SetMinExtent(glm::vec3 point);
	void SetMaxExtent(glm::vec3 point);
	void TranslateMinExtent(glm::vec3 amount);
	void TranslateMaxExtent(glm::vec3 amount);
	HitInfo OnCollision( BoxCollider &other);
	std::string GetObjectName();
private:
	std::string objectName;
	glm::vec3 mMinExtent;
	glm::vec3 mMaxExtent;
};
