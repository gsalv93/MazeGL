#pragma once
#include "Includes.h"
#include "Menu.h"
//Subclass of Menu
class MainMenu : public Menu
{
public:
	//Class constructor and destructor
	MainMenu() {};
	MainMenu(GLfloat h, GLfloat w);
	~MainMenu() {};
	//Draw menu method used in the glutDisplayFunc callback when the game state is MainMenu 
	void drawMenu(void);
	//Method used in the glutPassiveMotionFunc callback.
	void pickButton(long, long);
	//Method used to load texture buttons.
	void loadButtons();
	//getter that returns the selected button
	int getPressedButton() { return this->pressedButton; }
private:
	//Buttons textures
	GLuint buttonsTextures[3];
	GLuint buttonsTextureHover[3];
	//Buttons bounding boxes
	polygon buttons[3];
	//Pressed button index
	int pressedButton;
};

