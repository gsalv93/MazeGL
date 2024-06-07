#pragma once
#include "Includes.h"
#include "Window.h"
#include "Player.h"
#include "Maze.h"
#include "MazeBlockWall.h"
#include "PauseMenu.h"
#include "GameClearMenu.h"
#include "Key.h"

//Main game state
//All the game logic and the 3D maze interaction are handled in this class.
class Game
{
public:
	//Class Constructors and Destructor
	Game() {};
	Game(string, Difficulty, int, PauseMenu, GameClearMenu, ISoundEngine *, GLfloat, GLfloat);
	~Game() {};
	//Draw menu method used in the glutDisplayFunc callback when the game state is Game 
	void drawGameScene(void);

	//Method used to start the game and initialize every game-related stuff.
	void start();

	//Setters and Getters
	void setGameTime(int gameTime) { this->gameTime = gameTime; }
	void setIsPaused(bool isPaused) { this->isPaused = isPaused; }
	void setIsStarted(bool isStarted) { this->isStarted = isStarted; }
	void setTimeToDisplayMessages(int timeToDisplayMessages) { this->timeToDisplayMessages = timeToDisplayMessages; }
	void setRotation(int rotation) { this->rotation = rotation; }
	Player& getPlayer() { return this->player; }
	Maze& getMaze() { return this->maze; }
	Difficulty& getDifficulty() { return this->difficulty; }
	PauseMenu& getPauseMenu() { return this->pauseMenu; }
	GameClearMenu& getGameClearMenu() { return this->gameClearMenu; }
	int getGameTime() { return this->gameTime; }
	int getTimeToDisplayMessages() { return this->timeToDisplayMessages; }
	bool getIsPaused() { return this->isPaused; }
	bool getIsStarted() { return this->isStarted; }
	int getRotation() { return this->rotation; }
	
private:
	//Player object
	Player player;
	//Maze object
	Maze maze;
	//Difficulty level chosen
	Difficulty difficulty;
	//Pause menu object
	PauseMenu pauseMenu;
	//Game clear menu object
	GameClearMenu gameClearMenu;
	//Offsets required to properly scale the HUD in different screen sizes.
	GLfloat heightOffset;
	GLfloat widthOffset;
	//These are all textures used in the maze and the HUD.
	GLuint mazeTextures[3];
	GLuint startTexture;
	GLuint finishTexture;
	GLuint hudElements[3];
	GLuint hudIcons[2];
	//3D models of the key and treasures
	Model keyModel;
	Model diamondModel;
	//Sound IrrKlang engine (which will be the same one of the main)
	ISoundEngine* engine;
	//The player's name is first stored here.
	string playerName;
	//Rotation of the 3D model. Its value is increased periodically thanks to the timer function.
	//Thanks to this attribute, each 3D objects will rotate inside the maze, making them more distinct.
	int rotation;
	//Style of the maze (Classic, Futuristic or Vaporwave)
	int style;
	//Time passed expressed in seconds. This is also displayed on the HUD.
	int gameTime;
	//Used to display a message for n seconds
	int timeToDisplayMessages;
	//If the player was fast enough, they'll receive bonus points
	int bonusThreshold;
	//call list indices. Since these objects are static, they can be drawn once and then cached on the GPU
	//for a significant performance boost compared to drawing everything every frame.
	//Index of every wall block
	unsigned int walls;
	//Index of the four external walls, the ceiling and the floor
	unsigned int externalWalls;
	//Index of the entry of the maze
	unsigned int entry;
	//Index of the exit of the maze
	unsigned int exit;
	//Array containing a flagto check if a sound effect was played.
	//This is needed because otherwise the sound effect may be played multiple times and it *will* be
	//particularly displeasant on the ears. Trust me on this one.
	bool audioPlayed[4];
	//Flag to check if the game is paused
	bool isPaused;
	//Flag to check if the game is started
	bool isStarted;
	//Method used to load every textures.
	void loadTextures(string, GLuint *);
	//Method used to initialize the maze, starting from the difficulty level.
	//The harder the difficulty level, the bigger will be the generated maze.
	void initializingMaze(Difficulty);
	//This method is used to setup the game engine and to load textures and 3D models.
	void setup(void);
	//This method represents the first person camera that moves around the maze (however, as we know,
	//it's the maze that moves around the camera)
	void camera();
	//Method to manage the collision of the player against a wall.
	void managePlayerCollisionWithWall();
	//Method used to draw the 2D HUD on the screen.
	void drawHUD();
	//This method groups together all of the "loadTextures" calls.
	void loadingTextures();
	//This method sets up every display list
	void preparingDisplayLists();

};