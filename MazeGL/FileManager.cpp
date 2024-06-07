#include "FileManager.h"


void FileManager::readFile(Difficulty difficulty) {
	ifstream file;
	//Opening the csv file containing the rankings, based on the difficulty level requested.
	string path = "Rankings";
	if (difficulty == Difficulty::Easy)
		path.append("/easy.csv");
	else if (difficulty == Difficulty::Normal)
		path.append("/normal.csv");
	else if (difficulty == Difficulty::Hard)
		path.append("/hard.csv");

	file.open(path, ios::in); //Opening in read mode
	char c;
	while (file.good()) {//Iterating until the end of the file is reached.
		if (file.peek() == std::ifstream::traits_type::eof())
			break;
		file >> c;
		fileContents.push_back(c);
		if (file.peek() == '\n') //If a newline character is encountered, I push it back on the fileContents vector
			fileContents.push_back('\n');
	}
	if(!fileContents.empty())
		parseFile(); //The unparsed string from the file gets parsed.

	fileContents.clear(); //I clear the fileContents string for future file reads.
	file.close(); //Closing the file once I'm done.
}

void FileManager::writeToFile(Difficulty difficulty, string name, int time, int score) {
	ifstream fileRead;

	PlayerScore newScore = PlayerScore(name, time, score);
	
	//Opening the csv file containing the rankings, based on the difficulty level requested.
	string path = "Rankings";
	if (difficulty == Difficulty::Easy)
		path.append("/easy.csv");
	else if (difficulty == Difficulty::Normal)
		path.append("/normal.csv");
	else if (difficulty == Difficulty::Hard)
		path.append("/hard.csv");

	fileRead.open(path, ios::in); //Opening in read mode and write mode
	char c;
	while (fileRead.good()) {//Iterating until the end of the file is reached.
		if (fileRead.peek() == std::ifstream::traits_type::eof())
			break;
		fileRead >> c;
		fileContents.push_back(c);
		if (fileRead.peek() == '\n') //If a newline character is encountered, I push it back on the fileContents vector
			fileContents.push_back('\n');
	}
	if (!fileContents.empty())
		parseFile();

	fileRead.close(); //Closing the file once I'm done.
	//After all that, I add the new scores
	this->scores.push_back(newScore);

	//I sort the scores, and, finally, I write them onto the file.
	std::sort(scores.begin(), scores.end(), [](PlayerScore a, PlayerScore b) {
		return (b.getScore() < a.getScore());
		});

	
	ofstream fileWrite;
	fileWrite.open(path, std::ios::out | std::ios::trunc);// clear contents

	int size = scores.size();
	//I make sure to have a scores vector with the size of 8, by removing the final entries
	//(since the vector is sorted, the final entries have low scores anyways)
	if (size > 8) {
		size = 8;
		scores.resize(size);
	}		
	
	for (int i = 0; i < size; i++) // access by reference to avoid copying
	{
		//Writing the sorted vector elements in the file
		fileWrite << scores.at(i).getName();
		fileWrite << ',';
		fileWrite << scores.at(i).getTime();
		fileWrite << ',';
		fileWrite << scores.at(i).getScore();
		if(i < size-1)
			fileWrite << '\n';
	}

	fileWrite.close(); //Closing the file once I'm done.
}

void FileManager::parseFile() {
	//I clean the scores vector, in order to properly populate it
	scores.clear();
	//Once the file is successfully loaded in, we can parse its contents inside a Score vector.
	
	//The csv format is as follows: PLAYERNAME, TIME, SCORE
	//In order to properly parse this file, I'll iterate through the fileContents string and,
	//thanks to the stringstream object, I can extract the player's name, the time they took to
	//complete the maze and the score they achieved.
	string tmpName;
	string tmpTime;
	string tmpScore;
	stringstream stringStream(fileContents);
	vector<string> tempValues;
	while (stringStream.good()) { //While a newline character or the eof is reached
		tmpName = "";
		tmpTime = "";
		tmpScore = "";
		//I obtain the values thanks to getline.
		getline(stringStream, tmpName, ',');
		getline(stringStream, tmpTime, ',');
		getline(stringStream, tmpScore, '\n');
		//I can then create a PlayerScore object, containing tmpName, tmpTime and tmpScore values.
		scores.push_back(PlayerScore(tmpName, stoi(tmpTime), stoi(tmpScore)));
	}

	//Sorting the scores vector by score.
	std::sort(scores.begin(), scores.end(), [](PlayerScore a, PlayerScore b) {
		return (b.getScore() < a.getScore());
		});
}
