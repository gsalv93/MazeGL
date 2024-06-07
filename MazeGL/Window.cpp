#include "Window.h"

//Window constructor.
Window::Window() {
	
	window_xPos = 0;
	window_yPos = 0;
	z_far = 200;
	z_near = 1;
}

//Mathod used to initialize the window, based on the parameter used in the constructor.
//The double buffer, depth buffer and transparency are all activated.
void Window::windowSetup() {
	windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
	windowWidth = glutGet(GLUT_SCREEN_WIDTH);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(window_xPos, window_yPos);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glutCreateWindow("MazeGL");
}