#pragma once
#include "Includes.h"
#include "Menu.h"
class GameSetUpMenu :
    public Menu
{
public:
	//GameSetupMenu constructor and destructor
	GameSetUpMenu() {};
	GameSetUpMenu(GLfloat, GLfloat);
	~GameSetUpMenu() {};
	//Draw menu method used in the glutDisplayFunc callback when the game state is GameSetUpMenu 
	void drawMenu(void);
	//Method used in the glutPassiveMotionFunc callback.
	void pickButton(long, long);
	//Method used to load texture buttons.
	void loadButtons();
	//Method used to check if the game is ready to start.
	bool checkIfGameIsReady();
	//Method to set every button to not hovered.
	void resetButtons();

	//Setters and Getters
	void setPlayerName(string playerName) { this->playerName = playerName; }
	void setDifficultyLevel(Difficulty difficultyLevel) { this->difficultyLevel = difficultyLevel; }
	void setStyle(int style) { this->style = style; }
	void setIsTextBoxEnabled(bool isTextBoxEnabled) { this->isTextBoxEnabled = isTextBoxEnabled; }
	string getPlayerName() { return this->playerName; }
	Difficulty getDifficultyLevel() { return this->difficultyLevel; }
	int getStyle() { return this->style; }
	int getPressedButton() { return this->pressedButton; }
	bool getIsTextBoxEnabled() { return this->isTextBoxEnabled; }


private:
	//Difficulty level which will make the maze smaller or bigger
	Difficulty difficultyLevel;
	//Buttons textures and bounding boxes.
	GLuint buttonsTextures[5];
	GLuint buttonsTextureHover[5];
	polygon buttons[9];
	polygon textBox;
	polygon styleIcons[3];
	//Player name
	string playerName;
	//Maze style
	int style;
	//Pressed button index
	int pressedButton;
	//Flag to check if the text box is enabled
	bool isTextBoxEnabled;
};

