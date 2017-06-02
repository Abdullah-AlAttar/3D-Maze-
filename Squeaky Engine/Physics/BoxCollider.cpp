#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
	this->mMaxExtent = this->mMinExtent = glm::vec3(0, 0, 0);
}

BoxCollider::BoxCollider(glm::vec3 _mMinExtent, glm::vec3 _mMaxExtent, std::string name)
{
	this->mMaxExtent = _mMaxExtent;
	this->mMinExtent = _mMinExtent;
	this->objectName = name;
}

glm::vec3 BoxCollider::GetMinExtent()
{
	return mMinExtent;
}

glm::vec3 BoxCollider::GetMaxExtent()
{
	return mMaxExtent;
}
void BoxCollider::SetMinExtent(glm::vec3 point)
{
	this->mMinExtent = point;
}
void BoxCollider::SetMaxExtent(glm::vec3 point)
{
	this->mMaxExtent = point;
}
void BoxCollider::TranslateMinExtent(glm::vec3 amount)
{
	this->mMinExtent += amount;
}
void BoxCollider::TranslateMaxExtent(glm::vec3 amount)
{
	this->mMaxExtent += amount;
}
#include <iostream>
using namespace std;
HitInfo BoxCollider::OnCollision( BoxCollider & other)
{
	glm::vec3 distance1 = other.GetMinExtent()-this->mMaxExtent;
	glm::vec3 distance2 = this->mMinExtent-other.GetMaxExtent();
	glm::vec3 distance = glm::vec3(glm::max(distance1.x, distance2.x), glm::max(distance1.y, distance2.y), glm::max(distance1.z, distance2.z));
	float maxDistance = glm::max(distance.x, glm::max(distance.y, distance.z));
	return HitInfo(maxDistance < 0, maxDistance,other.GetObjectName());
}

std::string BoxCollider::GetObjectName()
{
	return objectName;
}
