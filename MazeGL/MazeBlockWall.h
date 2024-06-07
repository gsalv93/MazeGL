#pragma once
#include "Includes.h"

//MazeBlockWall class contains information regarding every wall block inside the maze. Each wall block is a cube centered in a specific position
//and with length of 10.
class MazeBlockWall
{
public:
	//Constructors and Destructor
	MazeBlockWall() {};
	MazeBlockWall(float, float, float);
	~MazeBlockWall() {};
	//DrawBlock method, which calls drawSolidCube and drawBox in order to draw a cube centered on the
	//(centerX, centerY, centerZ) point.
	void drawBlock();
	//Method to check collision with the player.
	bool checkCollisionWithPlayer(tuple<GLfloat, GLfloat>, float);
	//Getter methods
	float getCenterX() { return centerX; }
	float getCenterY() { return centerY; }
	float getCenterZ() { return centerZ; }
	float getLength() { return length; }
private:
	int length;
	float centerX, centerY, centerZ;
	void drawBox(GLenum);
	void drawSolidCube();
};