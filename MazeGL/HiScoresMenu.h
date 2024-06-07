#pragma once
#include "Includes.h"
#include "Menu.h"
#include "FileManager.h"
#include "PlayerScore.h"
//HiScoresMenu class contains every instruction needed to display the menu itself on the screen and to populate the
//difficulty tables with the best PlayerScores. Each table contains up to 8 entries.
class HiScoresMenu : public Menu
{
public:
	//Constructors and Destructor.
	HiScoresMenu() {};
	HiScoresMenu(array<FileManager,3>, GLfloat, GLfloat);
	~HiScoresMenu() {};
	//Draw function.
	void drawMenu(void);
	//Method called by the mouseTracker callback.
	void pickButton(long, long);
	//Method used to load the buttons textures.
	void loadButtons();
	//Setter and getters.
	//Setting a different difficulty stat will change the window looks. For this reason, the BB have to be rearranged.
	void setDifficultyStats(Difficulty difficultyStats) { this->difficultyStats = difficultyStats; rearrangeBB(); }
	void getScoresFromFiles();
	int getPressedButton() { return this->pressedButton; }
private:
	//RearrangeBB changes the bonding boxes for mouse pointer and button collision detection when a difficulty button is
	//pressed.
	void rearrangeBB();
	//Button textures, along with their hover version and their BB
	GLuint buttonsTextures[4];
	GLuint buttonsTextureHover[4];
	polygon buttons[4];
	//Pressed button index
	int pressedButton;
	//Selected difficulty
	Difficulty difficultyStats;
	//Scores for the three difficulties. They are collected in an array of vector in order to avoid
	//multiple if statements.
	vector<PlayerScore> scores[3];
	//File manager for each difficulty level.
	array<FileManager,3> scoresFileManager;
};

