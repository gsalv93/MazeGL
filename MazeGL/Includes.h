#pragma once
#pragma comment(lib, "irrKlang.lib")
//This header file contains every library, constant and function declarations.
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stack>
#include <tuple>
#include <array>
#include <vector>
#include <random>
#include <cmath>
#include <map>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Object/Model.h"
#include "soil.h"
#include "irrKlang.h"

//used namespaces
using namespace std;
using namespace objloader;
using namespace irrklang;

//Some constants
#define FPS 60
#define PI 3.14159265
#define TO_RADIANS PI/180.0

//Required for smooth movement with keyboard
struct Motion{
	bool Forward, Backward, Left, Right;
};

//Easy -> 7x7
//Normal -> 9x9
//Hard -> 11x11
//Enum used for difficulty levels
enum class Difficulty {
	Easy = 0,
	Normal = 1,
	Hard = 2,
	NoDifficultyChosen = 3
};

//Enum used for game states
enum class GameState {
	MainMenu = 0,
	SettingGameUp = 1,
	Game = 2,
	HiScores = 3
};

//struct for button bounding boxes.
typedef struct polygon{
	bool hover; //TRUE if polygon exists
	int xmin, xmax, ymin, ymax; /* bounding box */
} polygon;

//All of the function declarations.
static void resize(int, int);
static void drawGameScene(void);
static void key(unsigned char, int, int);
static void menusKeyboardFunc(unsigned char, int, int);
static void gamePausedInputDown(unsigned char, int, int);
static void gameKeyInputDown(unsigned char, int, int);
static void gameKeyInputUp(unsigned char, int, int);
static void inGamePassiveMotion(int, int);
static void timer(int);
void mouseTracker(int, int);
void mouseClick(int, int, int, int);
void writeStrokeString(void*, char*);
void setStuffForGame();
void applyOrthographicProjection();
void restorePerspectiveProjection();
void startGameSession(string, Difficulty);
void pauseGame();
void resumeGame();
void resetGameStats();
void restoringMainMenu();