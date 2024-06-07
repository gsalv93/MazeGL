#include "PlaneDrawer.h"

void PlaneDrawer::draw() {
	glPushMatrix();
	glBegin(GL_QUADS);
	//Texture must be repeated. For this reason, its coordinates are set beyond the interval [0,1].
	//I want the texture to be repeated by the number of tiles of the maze.
	//This means that I have to reverse the width calculation that happened in the PlaneDrawer constructor.
		glTexCoord2f(0, 0);
		glVertex3f(15, height, -15);

		glTexCoord2f(0, ((width + 15) / 10));
		glVertex3f(length, height, -15);
		
		glTexCoord2f(((width + 15) / 10), ((width + 15) / 10));
		glVertex3f(length, height, width);
		
		glTexCoord2f(((width + 15) / 10), 0);
		glVertex3f(15, height, width);
	glEnd();
	glPopMatrix();
}