#pragma once
#include <iostream>

struct HitInfo
{
	bool isColliding;
	float distance;
	std::string name;
public:
	HitInfo(bool _isCollding, float _distance,std::string _name) :
		isColliding(_isCollding), distance(_distance) ,name(_name){}
	bool IsColliding()
	{
		return 	isColliding;
	}
	float GetDistance()
	{
		return distance;
	}
	std::string GetName()
	{
		return name;
	}
};
