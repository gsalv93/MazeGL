#pragma once
#include "Includes.h"
class Player
{
public:
	//Constructors and Destructor
	Player() {};
	Player(string, pair<int, int>, int);
	~Player() {};

	//Setters and Getters
	void setCoordinates(GLfloat x_coordinate, GLfloat z_coordinate) { this->coordinates = make_pair(x_coordinate, z_coordinate); }
	void setYaw(GLfloat yaw) { this->yaw = yaw; }
	void setPitch(GLfloat pitch) { this->pitch = pitch; }
	void setScore(int score) { this->score = score; }
	void setNumberOfCollectedTreasures(int numberOfCollectedTreasures) { this->numberOfCollectedTreasures = numberOfCollectedTreasures; }
	void setIsKeyObtained(bool isKeyObtained) { this->isKeyObtained = isKeyObtained; }
	void setIsCollided(bool isCollided) { this->isCollided = isCollided; }
	void setForwardMotion(bool forwardMotion) { this->motion.Forward = forwardMotion; }
	void setBackwardMotion(bool backwardMotion) { this->motion.Backward = backwardMotion; }
	void setLeftMotion(bool leftMotion) { this->motion.Left = leftMotion; }
	void setRightMotion(bool rightMotion) { this->motion.Right = rightMotion; }
	void setHasWon(bool hasWon) { this->hasWon = hasWon; }
	
	pair<GLfloat, GLfloat> getCoordinates() { return this->coordinates; }
	GLfloat getYaw() { return this->yaw; }
	GLfloat getPitch() { return this->pitch; }
	string getName() { return this->name; }
	int getScore() { return this->score; }
	int getNumberOfCollectedTreasures() { return this->numberOfCollectedTreasures; }
	float getBoundingSphereRadius() { return this->boundingSphereRadius; }
	bool getIsKeyObtained() { return this->isKeyObtained; }
	bool getIsCollided() { return this->isCollided; }
	bool getForwardMotion() { return this->motion.Forward; }
	bool getBackwardMotion() { return this->motion.Backward; }
	bool getLeftMotion() { return this->motion.Left; }
	bool getRightMotion() { return this->motion.Right; }
	bool getHasWon() { return this->hasWon; }
	//Method to print the player's information on the console (used for test purposes)
	void printStuff() {
		cout << "Name: " << name << endl << "Score: " << score << endl << "Coordinates: " << get<0>(coordinates) << ", " << get<1>(coordinates) << endl
			<<"Pitch: " <<pitch<< endl <<"Yaw: " << yaw << endl << "Motions: " << motion.Forward << ", " << motion.Backward << ", " << motion.Left << ", " << motion.Right << endl;
	}
private:
	Motion motion;
	pair<GLfloat, GLfloat> coordinates;
	GLfloat yaw;
	GLfloat pitch;
	string name;
	int score;
	int numberOfCollectedTreasures;
	bool isKeyObtained;
	bool isCollided;
	bool hasWon;	
	float boundingSphereRadius;
	//Method used to find the starting position of the player inside the maze.
	void findStartingPosition(pair<int, int>, int);
};

