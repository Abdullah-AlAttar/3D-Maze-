#pragma once
#include <GLFW\glfw3.h>
#include <iostream>
using namespace std;
class Time
{

public:
	static void Start();
	static void Update();
	static float GetDeltaTime();
	static float GetTime();
private:
	 static float currentTime;
	 static float lastTime;
	 static float deltaTime;
	 static int framesNum;
	 static float framesTimer;
};	