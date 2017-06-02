#pragma once
#include <iostream>
#include <string>
#include<GL\glew.h>
#include <GLFW\glfw3.h>
#include "..\Input\Input.h"
#pragma comment(lib,"opengl32.lib")
using namespace std;
class WindowManager
{
private:
	
	bool Initialize();
	static GLFWwindow *window;
public:
	static int WIDTH, HEIGHT, FPS_CAP;
	void Start();
	void BeginUpdate();
	void EndUpdate();
	void End();
	bool IsRunning();
};