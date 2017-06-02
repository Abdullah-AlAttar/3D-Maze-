#include "WindowManager.h"

int WindowManager::WIDTH = 1600;
int WindowManager::HEIGHT = 900;
int WindowManager::FPS_CAP = 120;
GLFWwindow *WindowManager::window = NULL;
bool WindowManager::Initialize()
{

	if (!glfwInit())
	{
		cout << "Error initializing  GLFW" << endl;

		return false;
	}
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	/*WIDTH = mode->width;
	HEIGHT = mode->height;*/
	window = glfwCreateWindow(WIDTH ,HEIGHT, "Squeaky Engine", NULL, NULL);
	glViewport(0, 0, WIDTH, HEIGHT);
	if (window == NULL)
	{
		cout << "Error Creating a window " << endl;
		return false;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << endl;
		return false;
	}
	glfwSetCursorPosCallback(window, Input::MousePosCallback);
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetScrollCallback(window, Input::MouseScrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

void WindowManager::Start()
{
	if(this->Initialize()==false)
	{
		cout << "Error starting Window " << endl;
	}	
}

void WindowManager::BeginUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glfwPollEvents();
	
}

void WindowManager::EndUpdate()
{
	glfwSwapBuffers(window);
}

void WindowManager::End()
{
	glfwTerminate();
}

bool WindowManager::IsRunning()
{
	return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}
