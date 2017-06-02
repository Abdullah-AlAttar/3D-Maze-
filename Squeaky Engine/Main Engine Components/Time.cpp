#include "Time.h"
float Time::deltaTime = 0;
float Time::currentTime = 0;
float Time::lastTime = 0;
float Time::framesTimer = 0;
int Time::framesNum = 0;
void Time::Start()
{
	lastTime = glfwGetTime();
}

void Time::Update()
{
    currentTime = glfwGetTime();
	deltaTime = (currentTime - lastTime);
	lastTime = currentTime;
	framesNum++;
	if (currentTime - framesTimer >= 1.0) { 
									
		cout <<framesNum << " Frames Per Second "<< endl;
		framesNum = 0;
		framesTimer += 1.0;
	}
}

float Time::GetDeltaTime()
{
	return deltaTime;
}

float Time::GetTime()
{
	return glfwGetTime();
}
