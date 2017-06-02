#include "Camera.h"





glm::mat4 AbstractCamera::GetViewMatrix()
{
	return m_ViewMatrix;
}

void AbstractCamera::SetupProjectionMatrix(float aspectRatio, float zNear, float zFar)
{
	m_aspectRatio = aspectRatio;
	m_zNear = zNear;
	m_zFar = zFar;
	m_ProjectionMatrix= glm::perspective(glm::radians(m_Zoom), m_aspectRatio, m_zNear, m_zFar);
}
glm::mat4 AbstractCamera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}
glm::vec3 AbstractCamera::GetCameraPosition()
{
	return m_Position;
}
glm::vec3 AbstractCamera::GetCameraFront()
{
	return m_Front;
}
void AbstractCamera::SetCameraPosition(glm::vec3 position)
{
	m_Position = position;
}
FreeCamera::FreeCamera(glm::vec3 position) 
{
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_MovementSpeed = SPEED;
	m_MouseSensitivity = SENSITIVTY;
	m_Zoom = ZOOM;
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Yaw = YAW;
	m_Pitch = PITCH;
	m_Position = position;
	m_WorldUp = m_Up;
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	m_ProjectionMatrix = glm::perspective(glm::radians(m_Zoom), m_aspectRatio, m_zNear, m_zFar);
	Update();
}
void FreeCamera::Update()
{
	glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(m_Yaw), glm::radians(m_Pitch), 0.0f);
	glm::vec3 front;
	/*front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));*/
	front = glm::vec3(rotationMatrix*glm::vec4(0, 0, 1, 0));
	m_Front = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp)); 
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	m_ProjectionMatrix = glm::perspective(glm::radians(m_Zoom), m_aspectRatio, m_zNear, m_zFar);
}
void FreeCamera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = m_MovementSpeed * deltaTime;
	if (direction == FORWARD)
		m_Position += m_Front * velocity;
	if (direction == BACKWARD)
		m_Position -= m_Front * velocity;
	if (direction == LEFT)
		m_Position -= m_Right * velocity;
	if (direction == RIGHT)
		m_Position += m_Right * velocity;
	if (direction == UPWARD)
		m_Position += m_WorldUp * velocity;
	if (direction == DOWNWARD)
		m_Position -= m_WorldUp * velocity;
	Update();
}
void FreeCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Pitch -= yoffset;
	m_Yaw -= xoffset;

	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}
	Update();
}

void FreeCamera::ProcessMouseScroll(GLfloat yoffset)
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yoffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;
}


FPSCamera::FPSCamera(glm::vec3 position)
{
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_MovementSpeed = SPEED;
	m_MouseSensitivity = SENSITIVTY;
	m_Zoom = ZOOM;
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Yaw = YAW;
	m_Pitch = PITCH;
	m_Position = position;
	m_WorldUp = m_Up;
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	Update();
}
void FPSCamera::Update()
{
	// Calculate the new Front vector
	glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(m_Yaw), glm::radians(m_Pitch), 0.0f);
	glm::vec3 front;
	/*front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));*/
	front = glm::vec3(rotationMatrix*glm::vec4(0, 0, 1, 0));
	
	m_Front = glm::normalize(front);
	//cout << m_Front.x << " " << m_Front.y << " " << m_Front.z << endl;
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
void FPSCamera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = m_MovementSpeed * deltaTime;
	GLfloat y = m_Position.y;
	if (direction == FORWARD)
		m_Position += m_Front * velocity;
	if (direction == BACKWARD)
		m_Position -= m_Front * velocity;
	if (direction == LEFT)
		m_Position -= m_Right * velocity;
	if (direction == RIGHT)
		m_Position += m_Right * velocity;
	m_Position.y = y;
}
void FPSCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Pitch -= yoffset;
	m_Yaw -= xoffset;

	if (constrainPitch)
	{
		if (m_Pitch > 50.0f)
			m_Pitch = 50.0f;
		if (m_Pitch < -50.0f)
			m_Pitch = -50.0f;
	}
	Update();
}

void FPSCamera::ProcessMouseScroll(GLfloat yoffset)
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yoffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;
}

TPCamera::TPCamera(glm::vec3 position)
{
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_MovementSpeed = SPEED;
	m_MouseSensitivity = SENSITIVTY;
	m_Zoom = ZOOM;
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Yaw = YAW;
	m_Pitch = PITCH;
	m_Position = position;
	m_WorldUp = m_Up;
	m_Distance = 50;
	m_MinimumDistance = 50;
	m_MaximumDistance = 50;
	Update();

}

void TPCamera::SetTarget(Transform target)
{
	m_TargetTransform = target;
	m_Distance = glm::distance(m_Position, m_TargetTransform.position);
	m_Distance = glm::clamp(m_Distance, m_MinimumDistance, m_MaximumDistance);
	Update();
}
void TPCamera::SetTarget(Transform target,float distance)
{
	m_TargetTransform = target;
	m_Distance = distance;
	Update();
}
void TPCamera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{

}

void TPCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	m_Pitch += yoffset*m_MouseSensitivity;
	m_Yaw -= xoffset*m_MouseSensitivity;
	if (constrainPitch)
	{
		if (m_Pitch > 60.0f)
			m_Pitch = 60.0f;
		if (m_Pitch < -60.0f)
			m_Pitch = -60.0f;
	}
	Update();
}

void TPCamera::ProcessMouseScroll(GLfloat yoffset)
{
	/*m_Position += m_Front * yoffset;
	m_Distance = glm::distance(m_Position, m_TargetTransform.position);
	m_Distance = glm::clamp(m_Distance, m_MinimumDistance, m_MaximumDistance);*/
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yoffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;
}

void TPCamera::Update()
{
	glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(m_Yaw), glm::radians(m_Pitch), 0.0f);
	glm::vec3 translation = glm::vec3(0, 0, m_Distance);
	translation = glm::vec3(rotationMatrix*glm::vec4(translation, 0.0f));
	m_Position = m_TargetTransform.position + translation;
	m_Front = glm::normalize(m_TargetTransform.position - m_Position);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	m_ViewMatrix = glm::lookAt(m_Position, m_TargetTransform.position, m_Up);
	m_ProjectionMatrix = glm::perspective(glm::radians(m_Zoom), m_aspectRatio, m_zNear, m_zFar);
}
void FixedCamera::Update()
{
	glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(m_Yaw), glm::radians(m_Pitch), 0.0f);
	glm::vec3 translation = glm::vec3(0, 0, m_Distance);
	translation = glm::vec3(rotationMatrix*glm::vec4(translation, 0.0f));
	m_Position = m_TargetTransform.position + translation;
	m_Front = glm::normalize(m_TargetTransform.position - m_Position);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	m_ViewMatrix = glm::lookAt(m_Position, m_TargetTransform.position, m_Up);
	m_ProjectionMatrix = glm::perspective(glm::radians(m_Zoom), m_aspectRatio, m_zNear, m_zFar);
}

FixedCamera::FixedCamera(glm::vec3 position)
{
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_MovementSpeed = SPEED;
	m_MouseSensitivity = SENSITIVTY;
	m_Zoom = ZOOM;
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Yaw = 0;
	m_Pitch = -95;
	m_Position = position;
	m_WorldUp = m_Up;
	m_Distance = 40;
	m_MinimumDistance = 40;
	m_MaximumDistance = 40; 
	Update();
}

void FixedCamera::SetTarget(Transform targetTransform)
{
	m_TargetTransform = targetTransform;
	m_Distance = glm::distance(m_Position, m_TargetTransform.position);
	m_Distance = glm::clamp(m_Distance, m_MinimumDistance, m_MaximumDistance);
	Update();
}

void FixedCamera::SetTarget(Transform targetTransform,float distance)
{
	m_TargetTransform = targetTransform;
	m_Distance = distance;
	Update();
}

void FixedCamera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
}

void FixedCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	m_Pitch += yoffset*m_MouseSensitivity;
	m_Yaw -= xoffset*m_MouseSensitivity;
	if (constrainPitch)
	{
		if (m_Pitch > 60.0f)
			m_Pitch = 60.0f;
		if (m_Pitch < -60.0f)
			m_Pitch = -60.0f;
	}
	Update();
}

void FixedCamera::ProcessMouseScroll(GLfloat yoffset)
{
}

