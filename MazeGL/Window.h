#pragma once
#include "Includes.h"
class Window
{
public:
	//Constructor and Destructor
	Window();
	~Window() {};
	//Method used to setup the window so that OpenGL application can be drawn inside of it
	void windowSetup();

	//Getter methods	
	GLfloat getWindow_xPos() { return window_xPos; };
	GLfloat getWindow_yPos() { return window_yPos; };
	GLfloat getZ_far() { return z_far; };
	GLfloat getZ_near() { return z_near; };
	int getWindowHeight() { return windowHeight; };
	int getWindowWidth() { return windowWidth; };
private:
	//FreeGLUT window parameters
	GLfloat window_xPos;
	GLfloat window_yPos;
	GLfloat z_far;
	GLfloat z_near;
	int windowHeight;
	int windowWidth;
};

