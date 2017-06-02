#pragma once
#include "..\Main Engine Components\WindowManager.h"
#include <GLFW\glfw3.h>
#include<glm\glm.hpp>
class Input
{
public:
	static void MousePosCallback(GLFWwindow* window, double _x, double _y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static double  GetMouseX();
	static double  GetMouseY();
	static double  GetOffsetX();
	static double  GetOffsetY();
	static double  GetDeltaX();
	static double  GetDeltaY();
	static bool GetMouseButtonDown(int button);
	static bool GetMouseButtonUp(int button);
	static bool GetMouseButton(int button);
	static bool GetKeyDown(int keyCode);
	static bool GetKeyUp(int keyCode);
	static bool GetKey(int keyCode);
	static glm::vec2  FilterMouseMoves(float dx, float dy);
private:
	static double xMousePos;
	static double yMousePos;
	static double xOffset;
	static double yOffset;
	static double xDelta;
	static double yDelta;
	static bool mouseButtons[];
	static bool mouseButtonsDown[];
	static bool mouseButtonsUp[];
	static double xMousePosPrevious;
	static double yMousePosPrevious;
	static bool keys[];
	static bool keysDown[];
	static bool keysUp[];
	static glm::vec2 mouseHistory[];
};