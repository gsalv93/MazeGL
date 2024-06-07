#include "PlayerScore.h"
//Class constructor.
PlayerScore::PlayerScore(string name, int time, int score) {
	this->name = name;
	this->time = time;
	this->score = score;
}
//Print method that displays the class parameters on the console.
void PlayerScore::print() {
	cout << "Name: " << name << ", Time: "<< time<<", Score: "<< score<<endl;
}