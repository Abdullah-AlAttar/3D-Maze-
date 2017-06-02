#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "..\Input\Input.h"
#include <glm/gtx/euler_angles.hpp>
#include <algorithm>
#include "..\\Entities\GameObject.h"
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UPWARD,
	DOWNWARD
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.15f;
const GLfloat ZOOM = 45.0f;
const GLfloat DISTANCE_FROM_TARGET = 50;
const GLfloat MINUMUM_DISTANCE_FROM_TARGET = -60;
const GLfloat MAXIMUM_DISTANCE_FROM_TARGET = 60;
class AbstractCamera
{
public:
	glm::mat4 GetViewMatrix();
	void SetupProjectionMatrix(float aspectRatio, float zNear, float zFar);
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraFront();
	void SetCameraPosition(glm::vec3 position);
protected:

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	// Camera Attributes
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	// Eular Angles
	GLfloat m_Yaw;
	GLfloat m_Pitch;
	// Camera options
	GLfloat m_MovementSpeed;
	GLfloat m_MouseSensitivity;
	GLfloat m_Zoom;
	GLfloat m_aspectRatio;
	GLfloat m_zNear;
	GLfloat m_zFar;
	virtual void Update() = 0;
};

class FreeCamera :public AbstractCamera
{
public:
	FreeCamera(glm::vec3 position);
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yoffset);
	
	void Update();
};
class FPSCamera :public AbstractCamera
{
public:
	FPSCamera(glm::vec3 position);
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yoffset);
	void Update();
};
class TPCamera :public AbstractCamera
{
public:
	TPCamera(glm::vec3 position);
	void SetTarget(Transform targetTransform);
	void SetTarget(Transform target, float distance);
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yoffset);
	void Update();
private:
	Transform m_TargetTransform;
	GLfloat m_Distance;
	GLfloat m_MinimumDistance;
	GLfloat m_MaximumDistance;
};

class FixedCamera :public AbstractCamera
{
public:
	FixedCamera(glm::vec3 position);
	void SetTarget(Transform targetTransform);
	void SetTarget(Transform targetTransform, float distance);
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yoffset);
	void Update();
private:
	Transform m_TargetTransform;
	GLfloat m_Distance;
	GLfloat m_MinimumDistance;
	GLfloat m_MaximumDistance;
};



