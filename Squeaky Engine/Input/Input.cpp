
#include "Input.h"
const float MOUSE_FILTER_WEIGHT = 0.75f;
const int MOUSE_HISTORY_BUFFER_SIZE = 10;
double Input::xMousePos = 0;
double Input::yMousePos = 0;
double Input::xDelta = 0;
double Input::yDelta = 0;
double Input::xMousePosPrevious = WindowManager::WIDTH / 2;
double Input::yMousePosPrevious = WindowManager::HEIGHT / 2;
double Input::xOffset = 0;
double Input::yOffset = 0;
bool Input::mouseButtons[GLFW_MOUSE_BUTTON_LAST] = {};
bool Input::mouseButtonsUp[GLFW_MOUSE_BUTTON_LAST] = {};
bool Input::mouseButtonsDown[GLFW_MOUSE_BUTTON_LAST] = {};
bool Input::keys[GLFW_KEY_LAST] = {};
bool Input::keysDown[GLFW_KEY_LAST] = {};
bool Input::keysUp[GLFW_KEY_LAST] = {};
glm::vec2 Input::mouseHistory[MOUSE_HISTORY_BUFFER_SIZE];


void Input::MousePosCallback(GLFWwindow * window, double _x, double _y)
{

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	xMousePos = _x;
	yMousePos = height - _y;
	xDelta = (xMousePos - xMousePosPrevious);
	yDelta = (yMousePos - yMousePosPrevious);
	xMousePosPrevious = xMousePos;
	yMousePosPrevious = yMousePos;
}

void Input::MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (button < 0)
		return;
	if (action != GLFW_RELEASE && mouseButtons[button] == false)
	{
		mouseButtonsDown[button] = true;
		mouseButtonsUp[button] = false;
	}
	if (action == GLFW_RELEASE && mouseButtons[button] == true)
	{
		mouseButtonsDown[button] = false;
		mouseButtonsUp[button] = true;
	}
	mouseButtons[button] = action != GLFW_RELEASE;
}

void Input::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key < 0)
		return;
	if (action !=  GLFW_RELEASE && keys[key] == false)
	{
		
		keysDown[key] = true;
		keysUp[key] = false;
	}
	if (action == GLFW_RELEASE && keys[key] == true)
	{
		keysDown[key] = false;
		keysUp[key] = true;
	}
	keys[key] = action != GLFW_RELEASE;
}
glm::vec2 Input::FilterMouseMoves(float dx, float dy) {
	for (int i = MOUSE_HISTORY_BUFFER_SIZE - 1; i > 0; --i) {
		mouseHistory[i] = mouseHistory[i - 1];
	}

	// Store current mouse entry at front of array.
	mouseHistory[0] = glm::vec2(dx, dy);

	float averageX = 0.0f;
	float averageY = 0.0f;
	float averageTotal = 0.0f;
	float currentWeight = 1.0f;

	// Filter the mouse.
	for (int i = 0; i < MOUSE_HISTORY_BUFFER_SIZE; ++i)
	{
		glm::vec2 tmp = mouseHistory[i];
		averageX += tmp.x * currentWeight;
		averageY += tmp.y * currentWeight;
		averageTotal += 1.0f * currentWeight;
		currentWeight *= MOUSE_FILTER_WEIGHT;
	}
	return glm::vec2(averageX / averageTotal, averageY / averageTotal);
}

void Input::MouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{

	xOffset = xoffset;
	yOffset = yoffset;
}

double Input::GetMouseX()
{
	return xMousePos;
}

double Input::GetMouseY()
{
	return yMousePos;
}

double Input::GetOffsetX()
{
	double tmp = xOffset;
	xOffset = 0;
	return tmp;
}

double Input::GetOffsetY()
{
	double tmp = yOffset;
	yOffset = 0;
	return tmp;
}

double Input::GetDeltaX()
{
	double tmp = xDelta;
	xDelta = 0;
	return tmp;
}

double Input::GetDeltaY()
{
	double tmp = yDelta;
	yDelta = 0;
	return tmp;
}

bool Input::GetMouseButtonDown(int button)
{
	bool tmp = mouseButtonsDown[button];
	mouseButtonsDown[button] = false;

	return tmp;
}

bool Input::GetMouseButtonUp(int button)
{
	bool tmp = mouseButtonsUp[button];
	mouseButtonsUp[button] = false;

	return tmp;
}

bool Input::GetMouseButton(int button)
{
	return mouseButtons[button];
}

bool Input::GetKeyDown(int keyCode)
{
	bool tmp = keysDown[keyCode];
	keysDown[keyCode] = false;
	return tmp;
}

bool Input::GetKeyUp(int keyCode)
{
	bool tmp = keysUp[keyCode];
	keysUp[keyCode] = false;
	return tmp;
}

bool Input::GetKey(int keyCode)
{
	return keys[keyCode];
}



