#include "ExternalWall.h"

//This method calculates the coordinates of the wall based on its cardinal
//position.
void ExternalWall::calcCoordinates() {
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	switch (this->cardinalPosition) {
	case 0://NORTH
		x = 20;
		y = 0.0;
		z = 0.0;
		break;
	case 1://EAST
		x = 0.0;
		y = 0.0;
		z = -(getLength() - 5);
		break;
	case 2://SOUTH
		x = getLength() - 5;
		y = 0.0;
		z = 0.0;
		break;
	case 3://WEST
		x = 0;
		y = 0;
		z = -20;
		break;
	}
	this->setCoordinates(x, y, z);
}

//Checks if the player's coordinate exceed the labyrinth walls.
bool ExternalWall::checkCollisionWithPlayer(tuple<GLfloat, GLfloat> playerCoordinates) {
	float x_playerCoordinate = get<0>(playerCoordinates);
	float z_playerCoordinate = get<1>(playerCoordinates);

	//Checking the distance between the player and the wall, coordinate by coordinate.
	//If the player's coordinates exceed the wall coordinates, the player is considered collided.
	switch (this->cardinalPosition) {
	case 0://NORTH
		if (x_playerCoordinate < get<0>(this->getCoordinates())) {
			return true;
		}
		break;
	case 1://EAST
		if (z_playerCoordinate < get<2>(this->getCoordinates())) {
			return true;
		}
		break;
	case 2://SOUTH
		if (x_playerCoordinate > get<0>(this->getCoordinates())) {
			return true;
		}
		break;
		
	case 3://WEST
		if (z_playerCoordinate > get<2>(this->getCoordinates())) {
			return true;
		}
		break;
	}
	
	return false;
}

//Draw the wall, based on its cardinal position obviously.
void ExternalWall::draw() {
	switch (this->cardinalPosition) {
	case 0://NORTH
		this->drawNorthWall();
		break;
	case 1://EAST
		this->drawEastWall();
		break;
	case 2://SOUTH
		this->drawSouthWall();
		break;
	case 3://WEST
		this->drawWestWall();
		break;
	}
}
//These four methods are pretty self explanatory.
//They draw a wall on the extremal cardinal position of the maze.
//Once again, textures must be repeated. For this reason, their coordinates are set beyond (1,1).
//I want the texture to be repeated by the number of tiles of the maze.
//This means that I have to reverse the width calculation that happened in the ExternalWall constructor.
void ExternalWall::drawNorthWall() {
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(0, -5.0, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1);
			glVertex3f(15, 0, 15.0);
			
			glTexCoord2f(((getLength() - 15) / 10), 1);
			glVertex3f(getLength(), 0, 15.0);
			
			glTexCoord2f(((getLength() - 15) / 10), 0.0);
			glVertex3f(getLength(), getHeight(), 15.0);
			
			glTexCoord2f(0.0, 0.0);
			glVertex3f(15, getHeight(), 15.0);
		glEnd();
	glPopMatrix();
}

void ExternalWall::drawEastWall() {
	glPushMatrix();
		glTranslatef(0, -5.0, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1);
			glVertex3f(14.9, 0, -15.0);

			glTexCoord2f(((getLength() - 15) / 10), 1);
			glVertex3f(getLength(), -0.5, -15.0);

			glTexCoord2f(((getLength() - 15) / 10), 0.0);
			glVertex3f(getLength(), getHeight(), -15.0);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(14.9, getHeight(), -15.0);
		glEnd();
	glPopMatrix();
}

void ExternalWall::drawSouthWall() {
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glTranslatef(0, -5.0, (getLength() - 15.0));
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1);
			glVertex3f(15, 0, 14.9);

			glTexCoord2f(((getLength() - 15) / 10), 1);
			glVertex3f(getLength(), 0, 15.0);

			glTexCoord2f(((getLength() - 15) / 10), 0.0);
			glVertex3f(getLength(), getHeight(), 15.0);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(15, getHeight(), 15.0);
		glEnd();
	glPopMatrix();
}

void ExternalWall::drawWestWall() {
	glPushMatrix();
		glTranslatef(0, -5.0, -(getLength() - 15.0));
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1);
			glVertex3f(15.1, 0, -15.0);

			glTexCoord2f(((getLength() - 15) / 10), 1);
			glVertex3f(getLength(), 0, -15.0);

			glTexCoord2f(((getLength() - 15) / 10), 0.0);
			glVertex3f(getLength(), getHeight(), -15.0);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(15.1, getHeight(), -15.0);
		glEnd();
	glPopMatrix();
}