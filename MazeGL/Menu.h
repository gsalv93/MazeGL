#pragma once
#include "Includes.h"
//Superclass for all game menus.
class Menu
{
public:
	//Constructor and destructor
	Menu() {};
	Menu(GLfloat, GLfloat);
	~Menu() {};
	//Methods that will be overridden by subclasses
	void setupMenu(string);
	void drawMenu(void) {};
	void pickButton(long, long) {};
protected:
	//Method used to load textures
	void loadTextures(string, GLuint*);
	GLuint menuImage;
	GLfloat windowWidth;
	GLfloat windowHeight;
	GLfloat heightOffset;
	GLfloat widthOffset;
};

