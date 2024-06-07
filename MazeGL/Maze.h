#pragma once
#include "MazeBlockWall.h"
#include "Treasure.h"
#include "Key.h"
#include "StartTile.h"
#include "ExitTile.h"
#include "ExternalWall.h"

/// <summary>
/// Maze class which is going to generate the asacii maze, used by our OpenGL application.
/// The maze generation algorithm is achieved with recursive backtrack and is strongly inspired by
/// this github entry: https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_Mazes.cpp
/// When the maze is generating, it gets converted in ASCII characters, allowing for ease of use when recreating
/// the maze in 3D.
/// </summary>
class Maze {
public:
	//Basic constructor
	Maze() {};
	//Destructor
	~Maze() {};
	//Constructor with width and height parameters. This is the one that's actually used and it initializes
	//every class attribute.
	Maze(int, int);

	//This method is called when the maze is ready to get generated.
	bool generate();
	//Simple method to display the ascii maze in the console.
	void print();

	//Setter and Getters methods.
	StartTile& getStartTile() { return startTile; }
	ExitTile& getExitTile() { return exitTile; }
	Key& getKey() { return key; }
	PlaneDrawer& getCeiling() { return ceiling; }
	PlaneDrawer& getFloor() { return floor; }
	vector<MazeBlockWall>& getMazeBlocks() { return mazeBlocks; }
	vector<ExternalWall>& getWalls() { return walls; }
	map<int, Treasure>& getTreasures() { return treasures; }
	int getHeight() { return mazeHeight; }
	int getWidth() { return mazeWidth; }
	int getNumberOfTreasures() { return numberOfTreasures; }
	char* getAsciiMaze() { return asciiMaze; }
		
private:
	//StartTile object containing information regarding the starting position of the player inside the maze.
	StartTile startTile;
	//ExitTile object containing information regarding the exit position of the maze.
	ExitTile exitTile;
	//Key object containing information regarding its position inside the maze.
	Key key;
	//Celining and floor planes
	PlaneDrawer ceiling;
	PlaneDrawer floor;
	//Vector of MazeBlockWall objects. They are literally cubes that represent walls inside the maze.
	vector<MazeBlockWall> mazeBlocks;
	//Vector ov ExternallWall objects. They are the four external walls of the maze.
	vector<ExternalWall> walls;
	//Treasures Map (heh) containing information regarding their position inside the maze.
	//Here are up to "numberOfTreasures" treasures inside the maze.
	//The key is the index of a Treasure object. The value is the object itself.
	map<int, Treasure> treasures;
	// (x, y) coordinate pairs of the maze (this is used in the maze generation phase)
	stack<pair<int, int>> m_stack;
	//Number of visited cells. During the maze generation phase, the algorithm traverses the maze recursively and,
	//for each non visited tile that gets visited, this variable is increased. Once this variable is equal to
	//mazeWidth*mazeHeight, the maze generation algorithm will end.
	int visitedCells;
	//Width and height of the maze.
	int mazeWidth;
	int mazeHeight;
	//Number of treasures scattered inside the maze (it's set to 3)
	int numberOfTreasures;
	//Ascii maze that is used as a reference to generate the actual 3D maze. This array is populated
	//during the maze generation phase.
	char* asciiMaze;
	
	//enum tiles used in the MazeExploration private method
	enum
	{
		CELL_PATH_N = 0x01,
		CELL_PATH_E = 0x02,
		CELL_PATH_S = 0x04,
		CELL_PATH_W = 0x08,
		CELL_VISITED = 0x10,
	};
	//This method is used to check a neighbour of the tile located at the top of m_stack.
	int offset(int x, int y)
	{
		return (m_stack.top().second + y) * (mazeWidth / 2) + (m_stack.top().first + x);
	};
	//This method is pretty similar to offset above, but this is used when creating the ascii maze.
	//The int cast is required as "half steps" are taken along with a full step (reminder that
	//this ascii maze is twice as big as the standard maze).
	int ascii_offset(float x, float y){
		return int(((m_stack.top().second + y) * 2) * (mazeWidth + 1) + ((m_stack.top().first + x) * 2));
	};
	//Method used to generate the maze
	void mazeExploration(int*, int, int);
	//These methods are used to generate all the relevant tiles inside the maze
	//Start Door
	void generateStartDoor(int);
	//Exit Door
	void generateExitDoor(int);
	//Special tiles, that include...
	void specialTiles();
	//The Key position
	void keyTile(vector<pair<int, int>>&, char);
	//And the Treasures positions.
	void treasureTile(vector<pair<int, int>>&, char);

	//Finally, these two methods are used to setup the 3D maze.
	//In this method, all the planes are setup (Ceiling, Floor and external Walls)
	void setupPlanes();
	//In this method, the MazeBlockWalls vector is setup.
	void build3DMaze();
};