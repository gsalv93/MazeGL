#pragma once
#include "Includes.h"
#include "PlayerScore.h"
#include <fstream>
#include <sstream>

//Simple file manager class, allowing for file reading and writing. This will be used to save the best scored in three separate csv files.
//When the player wants to see the high scores, the three files are read, parsed and then displayed on screen with the thingy function.
//When the player ends a run, their score will be written to the csv file.
class FileManager
{
public:
	//Constructor and Destructor
	FileManager() {};
	~FileManager() {};
	//Method to read the file based on the difficulty level given in input.
	void readFile(Difficulty);
	//Method to write to the file based on the difficulty level given in input.
	void writeToFile(Difficulty, string, int, int);
	//Scores getter method
	vector<PlayerScore> getScores() { return this->scores; }
private:
	//Method used to parse the cvs inputted file.
	void parseFile();
	//Max 8 scores
	vector<PlayerScore> scores;
	//String that holds in all the file contents.
	string fileContents;
};

