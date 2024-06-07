#include "GameSetUpMenu.h"

//Class constructor where buttons bounding boxes are setup
GameSetUpMenu::GameSetUpMenu(GLfloat windowHeight, GLfloat windowWidth) : Menu(windowHeight, windowWidth) {
	//Initializing values
	this->playerName = "";
	this->difficultyLevel = Difficulty::NoDifficultyChosen;
	this->style = -1;

	//Easy button
	this->buttons[0].xmin = 1105 * this->widthOffset;
	this->buttons[0].xmax = 1344 * this->widthOffset;
	this->buttons[0].ymin = 435 * this->heightOffset;
	this->buttons[0].ymax = 510 * this->heightOffset;
	this->buttons[0].hover = 0;

	//Medium button
	this->buttons[1].xmin = 1105 * this->widthOffset;
	this->buttons[1].xmax = 1345 * this->widthOffset;
	this->buttons[1].ymin = 535 * this->heightOffset;
	this->buttons[1].ymax = 610 * this->heightOffset;
	this->buttons[1].hover = 0;

	//Hard button
	this->buttons[2].xmin = 1105 * this->widthOffset;
	this->buttons[2].xmax = 1345 * this->widthOffset;
	this->buttons[2].ymin = 635 * this->heightOffset;
	this->buttons[2].ymax = 710 * this->heightOffset;
	this->buttons[2].hover = 0;

	//Back button
	this->buttons[3].xmin = 493 * this->widthOffset;
	this->buttons[3].xmax = 734 * this->widthOffset;
	this->buttons[3].ymin = 875 * this->heightOffset;
	this->buttons[3].ymax = 950 * this->heightOffset;
	this->buttons[3].hover = 0;

	//Start button
	this->buttons[4].xmin = 1100 * this->widthOffset;
	this->buttons[4].xmax = 1380 * this->widthOffset;
	this->buttons[4].ymin = 840 * this->heightOffset;
	this->buttons[4].ymax = 955 * this->heightOffset;
	this->buttons[4].hover = 0;

	//Icons bounding boxes

	//Classic
	this->buttons[5].xmin = 515 * this->widthOffset;
	this->buttons[5].xmax = 625 * this->widthOffset;
	this->buttons[5].ymin = 595 * this->heightOffset;
	this->buttons[5].ymax = 700 * this->heightOffset;
	this->buttons[5].hover = 0;

	//Futuristic
	this->buttons[6].xmin = 660 * this->widthOffset;
	this->buttons[6].xmax = 765 * this->widthOffset;
	this->buttons[6].ymin = 595 * this->heightOffset;
	this->buttons[6].ymax = 700 * this->heightOffset;
	this->buttons[6].hover = 0;


	//Vaporwave
	this->buttons[7].xmin = 805 * this->widthOffset;
	this->buttons[7].xmax = 915 * this->widthOffset;
	this->buttons[7].ymin = 595 * this->heightOffset;
	this->buttons[7].ymax = 700 * this->heightOffset;
	this->buttons[7].hover = 0;


	//Text field bounding box
	this->isTextBoxEnabled = false;
	this->buttons[8].xmin = 500 * this->widthOffset;
	this->buttons[8].xmax = 965 * this->widthOffset;
	this->buttons[8].ymin = 445 * this->heightOffset;
	this->buttons[8].ymax = 500 * this->heightOffset;
	this->buttons[8].hover = 0;
}


//Draw method, used to display the menu
void GameSetUpMenu::drawMenu(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//Main menu bg
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, menuImage);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, this->windowHeight);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(this->windowWidth, this->windowHeight);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(this->windowWidth, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Drawing Buttons
	//Easy button
	glEnable(GL_TEXTURE_2D);
	
	if (this->buttons[0].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[0]);
	}
	if (this->buttons[0].hover == 1 || this->difficultyLevel == Difficulty::Easy) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[0]);
	}

	glPushMatrix();
	glTranslatef(1100.0 * this->widthOffset, 565.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 80.0 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(250.0 * this->widthOffset, 80.0 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(250.0 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//Medium button
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[1].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[1]);
	}

	if (this->buttons[1].hover == 1 || this->difficultyLevel == Difficulty::Normal) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[1]);
	}

	glPushMatrix();
	glTranslatef(1100 * this->widthOffset, 465.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 80.0 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(250.0 * this->widthOffset, 80.0 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(250.0 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	//Hard button
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[2].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[2]);
	}
	if (this->buttons[2].hover == 1 || this->difficultyLevel == Difficulty::Hard) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[2]);
	}

	glPushMatrix();
	glTranslatef(1100 * this->widthOffset, 365.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 80.0 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(250.0 * this->widthOffset, 80.0 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(250.0 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	//Back button
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[3].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[3]);
	}

	else if (this->buttons[3].hover == 1) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[3]);
	}

	glPushMatrix();
	glTranslatef(490 * this->widthOffset, 125.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 80.0 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(250.0 * this->widthOffset, 80.0 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(250.0 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	//Start button
	if (checkIfGameIsReady()) {
		glEnable(GL_TEXTURE_2D);
		if (this->buttons[4].hover == 0) {
			glBindTexture(GL_TEXTURE_2D, buttonsTextures[4]);
		}

		else if (this->buttons[4].hover == 1) {
			glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[4]);
		}

		glPushMatrix();
			glTranslatef(1100.0 * this->widthOffset, 125.0 * this->heightOffset, 0.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);
				glVertex2f(0.0, 0.0);
				glTexCoord2f(0.0, 1.0);
				glVertex2f(0.0, 110.0 * this->heightOffset);
				glTexCoord2f(1.0, 1.0);
				glVertex2f(285.0 * this->widthOffset, 110.0 * this->heightOffset);
				glTexCoord2f(1.0, 0.0);
				glVertex2f(285.0 * this->widthOffset, 0.0);
			glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	

	//Classic icon
	glPushMatrix();
		glColor3ub(192, 192, 192);
	if (this->buttons[5].hover == 1 || this->style == 1) {
		glColor3f(0, 0, 1);
	}
	glTranslatef(520.0 * this->widthOffset, 380.0 * this->heightOffset, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, 105.0 * this->heightOffset);
		glVertex2f(105.0 * this->widthOffset, 105.0 * this->heightOffset);
		glVertex2f(105.0 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();

	//Futuristic icon
	glPushMatrix();
		glColor3ub(192, 192, 192);
	if (this->buttons[6].hover == 1 || this->style == 2) {
		glColor3f(0, 0, 1);
	}
	glTranslatef(663.0 * this->widthOffset, 380.0 * this->heightOffset, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, 105.0 * this->heightOffset);
		glVertex2f(105.0 * this->widthOffset, 105.0 * this->heightOffset);
		glVertex2f(105.0 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();


	//Vaporwave icon
	glPushMatrix();
		glColor3ub(192, 192, 192);
	if (this->buttons[7].hover == 1 || this->style == 3) {
		glColor3f(0, 0, 1);
	}
	glTranslatef(808.0 * this->widthOffset, 380.0 * this->heightOffset, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, 105.0 * this->heightOffset);
		glVertex2f(105.0 * this->widthOffset, 105.0 * this->heightOffset);
		glVertex2f(105.0 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();

	//Text box
	glPushMatrix();
		glColor3ub(192, 192, 192);
		if (this->buttons[8].hover == 1 || this->isTextBoxEnabled) {
			glColor3f(0, 0, 1);
		}
		glTranslatef(500.0 * this->widthOffset, 570.0 * this->heightOffset, 0.0);
		glBegin(GL_LINE_LOOP);
			glVertex2f(0.0, 0.0);
			glVertex2f(0.0, 71.0 * this->heightOffset);
			glVertex2f(467.0 * this->widthOffset, 71.0 * this->heightOffset);
			glVertex2f(467.0 * this->widthOffset, 0.0);
		glEnd();
	glPopMatrix();
	//Text inside textbox
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(505.0 * this->widthOffset, 595.0 * this->heightOffset, 0.0);
		glScalef(0.25 * this->widthOffset, 0.25 * this->heightOffset, 1);
		writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)playerName.c_str());
	glPopMatrix();

	glutSwapBuffers();
}


void GameSetUpMenu::pickButton(long x, long y)
{
	//find first polygon in which we are in bounding box
	int i;
	//For loop iterating through every button in the menu.
	for (i = 0; i < 9; i++)
	{
		//For each button, a simple BB algorithm is applied (is the cursor inside the button BB?)
		if ((y >= this->buttons[i].ymin) && (y < this->buttons[i].ymax) && (x >= this->buttons[i].xmin) && (x <= this->buttons[i].xmax))
		{
			this->pressedButton = i; //save which button position mouse cursor is
			this->buttons[i].hover = 1; //give flag to this polygon, that the mouse cursor is over it
			glutPostRedisplay();
			break;

		}
		else
		{
			this->pressedButton = -1; //forget the position
			this->buttons[i].hover = 0; // remove the flag      
			glutPostRedisplay();
		}
	}
}


//If the game is ready, then the start button can be displayed.
//The game is ready if the name is written and when both the difficulty
//and style are chosen.
bool GameSetUpMenu::checkIfGameIsReady() {
	if (playerName.empty())
		return false;
	if (style == -1)
		return false;
	if(difficultyLevel == Difficulty::NoDifficultyChosen)
		return false;
	return true;
}

//resetting every button hover state
void GameSetUpMenu::resetButtons() {
	this->buttons[0].hover = 0;
	this->buttons[1].hover = 0;
	this->buttons[2].hover = 0;
	this->buttons[3].hover = 0;
	this->buttons[4].hover = 0;
	this->buttons[5].hover = 0;
	this->buttons[6].hover = 0;
	this->buttons[7].hover = 0;
	this->buttons[8].hover = 0;
}

//Loading buttons textures
void GameSetUpMenu::loadButtons() {
	glGenTextures(1, &buttonsTextures[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/easy.bmp", &buttonsTextures[0]);

	glGenTextures(1, &buttonsTextures[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/medium.bmp", &buttonsTextures[1]);

	glGenTextures(1, &buttonsTextures[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/hard.bmp", &buttonsTextures[2]);

	glGenTextures(1, &buttonsTextures[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/back.bmp", &buttonsTextures[3]);
	
	glGenTextures(1, &buttonsTextures[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/startmaze.bmp", &buttonsTextures[4]);

	
	
	glGenTextures(1, &buttonsTextureHover[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/easy_hover.bmp", &buttonsTextureHover[0]);
	
	glGenTextures(1, &buttonsTextureHover[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/medium_hover.bmp", &buttonsTextureHover[1]);
	
	glGenTextures(1, &buttonsTextureHover[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/hard_hover.bmp", &buttonsTextureHover[2]);
	
	glGenTextures(1, &buttonsTextureHover[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/back_hover.bmp", &buttonsTextureHover[3]);
	
	glGenTextures(1, &buttonsTextureHover[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/startmaze_hover.bmp", &buttonsTextureHover[4]);

}