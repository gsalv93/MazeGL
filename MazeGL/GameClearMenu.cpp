#include "GameClearMenu.h"

//Class constructor where buttons bounding boxes are setup
GameClearMenu::GameClearMenu(FileManager fileManager, GLfloat windowHeight, GLfloat windowWidth) : Menu(windowHeight, windowWidth) {
	
	this->button.xmin = 840 * this->widthOffset;
	this->button.xmax = 1080 * this->widthOffset;
	this->button.ymin = 560 * this->heightOffset;
	this->button.ymax = 640 * this->heightOffset;
	this->button.hover = 0;

	this->fileManager = fileManager;
}

//Draw method, used to display the menu
void GameClearMenu::drawMenu(void) {
	//Main Window
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, menuImage);
	
	glPushMatrix();
		glTranslatef(743.0 * this->widthOffset, 403.0 * this->heightOffset, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(0.0, 0.0);

			glTexCoord2f(0.0, 1.0);
			glVertex2f(0.0, 275.0 * this->heightOffset);

			glTexCoord2f(1.0, 1.0);
			glVertex2f(435.0 * this->widthOffset, 275.0 * this->heightOffset);

			glTexCoord2f(1.0, 0.0);
			glVertex2f(435.0 * this->widthOffset, 0.0);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//A message to the player is written inside the pop up window
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glPushMatrix();
			glTranslatef(760.0 * this->widthOffset, 615.0 * this->heightOffset, 0);
			glScalef(0.15 * this->widthOffset, 0.15 * this->heightOffset, 1);
			string text1 = "Well done, " + playerName + "!";
			writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)text1.c_str());	
		glPopMatrix();
		glPushMatrix();
			glTranslatef(760.0 * this->widthOffset, 585.0 * this->heightOffset, 0);
			glScalef(0.15 * this->widthOffset, 0.15 * this->heightOffset, 1);
			string text2 = "Here are your stats:";
			writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)text2.c_str());
		glPopMatrix();
		glPushMatrix();
			glTranslatef(760.0 * this->widthOffset, 555.0 * this->heightOffset, 0);
			glScalef(0.15 * this->widthOffset, 0.15 * this->heightOffset, 1);
			string text3 = "Time: " + to_string(gameTime);
			writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)text3.c_str());
		glPopMatrix();
		glPushMatrix();
			glTranslatef(760.0 * this->widthOffset, 530.0 * this->heightOffset, 0);
			glScalef(0.15 * this->widthOffset, 0.15 * this->heightOffset, 1);
			string text4 = "Score:" + to_string(score);
			writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)text4.c_str());
		glPopMatrix();		
	glPopMatrix();

	//Ok button
	glEnable(GL_TEXTURE_2D);
	if (this->button.hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonTexture);
	}
	else if (this->button.hover == 1) {
		glBindTexture(GL_TEXTURE_2D, buttonTextureHover);
	}
	glPushMatrix();
	glTranslatef(835.0 * this->widthOffset, 440.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 80 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(250.0 * this->widthOffset, 80 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(250.0 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}



void GameClearMenu::loadButton() {
	glGenTextures(1, &buttonTexture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/ok.bmp", &buttonTexture);

	glGenTextures(1, &buttonTextureHover);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/ok_hover.bmp", &buttonTextureHover);
}


void GameClearMenu::pickButton(long x, long y)
{
	//The BB algorithm is applied to the only button present on the window.
	if ((y >= this->button.ymin) && (y < this->button.ymax) && (x >= this->button.xmin) && (x <= this->button.xmax)){
		this->pressedButton = 0; //save which button position mouse cursor is
		this->button.hover = 1; //give flag to this polygon, that the mouse cursor is over it
		glutPostRedisplay();
	}
	else{
		this->pressedButton = -1; //forget the position
		this->button.hover = 0; // remove the flag      
		glutPostRedisplay();
	}
}

//This method saves the player's stats on the csv file of the corresponding difficulty
//thanks to the fileManager object.
void GameClearMenu::saveToFile() {
	fileManager.writeToFile(difficulty, playerName, gameTime, score);
}