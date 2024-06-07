#pragma once
#include "Includes.h"

//PlayerScores class contains all the information regarding the player's score. This info will be used to
//update the Hi-Scores screen if the player made it to the top 8.
class PlayerScore
{
public:
	//Constructors and Destructor
	PlayerScore() {};
	PlayerScore(string, int, int);
	~PlayerScore() {};
	void print();
	//Getter methods
	string getName(void) { return this->name; }
	int getTime(void) { return this->time; }
	int getScore(void) { return this->score; }
private:
	string name;
	int time;
	int score;
};

