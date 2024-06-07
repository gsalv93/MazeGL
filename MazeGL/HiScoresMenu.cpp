#include "HiScoresMenu.h"

//Simple constructor. Every button BB is set along with the difficulty level.
HiScoresMenu::HiScoresMenu(array<FileManager, 3> scoresFileManager, GLfloat windowHeight, GLfloat windowWidth) : Menu(windowHeight, windowWidth) {
	this->difficultyStats = Difficulty::Easy;
	
	this->buttons[0].xmin = 0;
	this->buttons[0].xmax = 0;
	this->buttons[0].ymin = 0;
	this->buttons[0].ymax = 0;
	this->buttons[0].hover = 0;

	this->buttons[1].xmin = 664 * this->widthOffset;
	this->buttons[1].xmax = 745 * this->widthOffset;
	this->buttons[1].ymin = 476 * this->heightOffset;
	this->buttons[1].ymax = 500 * this->heightOffset;
	this->buttons[1].hover = 0;

	this->buttons[2].xmin = 746 * this->widthOffset;
	this->buttons[2].xmax = 810 * this->widthOffset;
	this->buttons[2].ymin = 476 * this->heightOffset;
	this->buttons[2].ymax = 500 * this->heightOffset;
	this->buttons[2].hover = 0;

	this->buttons[3].xmin = 495 * this->widthOffset;
	this->buttons[3].xmax = 675 * this->widthOffset;
	this->buttons[3].ymin = 390 * this->heightOffset;
	this->buttons[3].ymax = 445 * this->heightOffset;
	this->buttons[3].hover = 0;
	this->scoresFileManager = scoresFileManager;
}




//Draw method, used to display the menu
void HiScoresMenu::drawMenu(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//Drawing the menu itself
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, menuImage);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, windowHeight);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(windowWidth, windowHeight);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(windowWidth, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//And, the buttons.
	//Choosing which texture to apply to the difficulty buttons.
	//Quick reminder:
	//0 -> easy
	//1 -> medium
	//2 -> hard
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, buttonsTextures[static_cast<int>(difficultyStats)]);
	//Drawing the button itself
	glPushMatrix();
	glTranslatef(578 * windowWidth / 1920, 574 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 33 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(231 * this->widthOffset, 33 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(231 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	//Putting the text in there too
	
	int spacing = 0;
	for (auto& score : scores[static_cast<int>(difficultyStats)]) // access by reference to avoid copying
	{
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(600.0 * this->widthOffset, (445.0 - spacing) * this->heightOffset, 0.0);
		glScalef(0.25 * this->widthOffset, 0.25 * this->heightOffset, 1);
		writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)score.getName().c_str());
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(875.0 * this->widthOffset, (445.0 - spacing) * this->heightOffset, 0.0);
		glScalef(0.25 * this->widthOffset, 0.25 * this->heightOffset, 1);
		writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)to_string(score.getTime()).c_str());
		glPopMatrix();


		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(1095.0 * this->widthOffset, (445.0 - spacing) * this->heightOffset, 0.0);
		glScalef(0.25 * this->widthOffset, 0.25 * this->heightOffset, 1);
		writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)to_string(score.getScore()).c_str());
		glPopMatrix();

		spacing += 45;

	}
	
	//Finally, drawing the back button
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[3].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[3]);
	}

	else if (this->buttons[3].hover == 1) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[3]);
	}
	glPushMatrix();
	glTranslatef(490 * this->widthOffset, 630.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 60 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(187 * this->widthOffset, 60 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(187 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

//When a button is pressed, the pressed one becomes bigger and the other two will get smaller.
//Hence, I have to change the BB of each button on the fly, to avoid potential misclicks.
void HiScoresMenu::rearrangeBB() {
	switch (difficultyStats) {
		case Difficulty::Easy: {
			this->buttons[0].xmin = 0;
			this->buttons[0].xmax = 0;
			this->buttons[0].ymin = 0;
			this->buttons[0].ymax = 0;

			this->buttons[1].xmin = 664 * this->widthOffset;
			this->buttons[1].xmax = 745 * this->widthOffset;
			this->buttons[1].ymin = 476 * this->heightOffset;
			this->buttons[1].ymax = 500 * this->heightOffset;

			this->buttons[3].xmin = 495 * this->widthOffset;
			this->buttons[3].xmax = 675 * this->widthOffset;
			this->buttons[3].ymin = 390 * this->heightOffset;
			this->buttons[3].ymax = 445 * this->heightOffset;
	}
		break;
		case Difficulty::Normal: {
			this->buttons[0].xmin = 580 * this->widthOffset;
			this->buttons[0].xmax = 640 * this->widthOffset;
			this->buttons[0].ymin = 476 * this->heightOffset;
			this->buttons[0].ymax = 500 * this->heightOffset;

			this->buttons[1].xmin = 0;
			this->buttons[1].xmax = 0;
			this->buttons[1].ymin = 0;
			this->buttons[1].ymax = 0;

			this->buttons[2].xmin = 730 * this->widthOffset;
			this->buttons[2].xmax = 790 * this->widthOffset;
			this->buttons[2].ymin = 476 * this->heightOffset;
			this->buttons[2].ymax = 500 * this->heightOffset;
		}
		break;
		case Difficulty::Hard: {
			this->buttons[0].xmin = 580 * this->widthOffset;
			this->buttons[0].xmax = 640 * this->widthOffset;
			this->buttons[0].ymin = 476 * this->heightOffset;
			this->buttons[0].ymax = 500 * this->heightOffset;

			this->buttons[1].xmin = 640 * this->widthOffset;
			this->buttons[1].xmax = 720 * this->widthOffset;
			this->buttons[1].ymin = 476 * this->heightOffset;
			this->buttons[1].ymax = 500 * this->heightOffset;

			this->buttons[2].xmin = 0;
			this->buttons[2].xmax = 0;
			this->buttons[2].ymin = 0;
			this->buttons[2].ymax = 0;
		}
		break;
	}
}



void HiScoresMenu::getScoresFromFiles() {
	this->scores[0].clear();
	this->scores[1].clear();
	this->scores[2].clear();
	//Reading from the three files and populating the scores vector
	this->scoresFileManager[0].readFile(Difficulty::Easy);
	this->scores[0] = this->scoresFileManager[0].getScores();
	this->scoresFileManager[1].readFile(Difficulty::Normal);
	this->scores[1] = this->scoresFileManager[1].getScores();
	this->scoresFileManager[2].readFile(Difficulty::Hard);
	this->scores[2] = this->scoresFileManager[2].getScores();
}


void HiScoresMenu::pickButton(long x, long y)
{
	//find first polygon in which we are in bounding box
	int i;
	//For loop iterating through every button in the menu.
	for (i = 0; i < 4; i++)
	{
		//For each button, a simple BB algorithm is applied (is the cursor inside the button BB?)
		if ((y >= this->buttons[i].ymin) && (y < this->buttons[i].ymax) && (x >= this->buttons[i].xmin) && (x <= this->buttons[i].xmax)){
			this->pressedButton = i; //save which button position mouse cursor is
			this->buttons[i].hover = 1; //give flag to this polygon, that the mouse cursor is over it

			glutPostRedisplay();
			break;

		}
		else{
			this->pressedButton = -1; //forget the position
			this->buttons[i].hover = 0; // remove the flag      
			glutPostRedisplay();
		}
	}

}


//Buttons textures are loaded in this method.
void HiScoresMenu::loadButtons() {
	glGenTextures(1, &buttonsTextures[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/easyhiscores.bmp", &buttonsTextures[0]);
	glGenTextures(1, &buttonsTextures[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/normalhiscores.bmp", &buttonsTextures[1]);
	glGenTextures(1, &buttonsTextures[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/hardhiscores.bmp", &buttonsTextures[2]);
	glGenTextures(1, &buttonsTextures[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/back.bmp", &buttonsTextures[3]);

	glGenTextures(1, &buttonsTextureHover[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/easyhiscores.bmp", &buttonsTextureHover[0]);
	glGenTextures(1, &buttonsTextureHover[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/normalhiscores.bmp", &buttonsTextureHover[1]);
	glGenTextures(1, &buttonsTextureHover[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/hardhiscores.bmp", &buttonsTextureHover[2]);
	glGenTextures(1, &buttonsTextureHover[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/back_hover.bmp", &buttonsTextureHover[3]);
}