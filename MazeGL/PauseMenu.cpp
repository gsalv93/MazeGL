#include "PauseMenu.h"

//Class constructor where buttons bounding boxes are setup
PauseMenu::PauseMenu(GLfloat windowHeight, GLfloat windowWidth) : Menu(windowHeight, windowWidth) {
	//Resume button
	this->buttons[0].xmin = 842 * this->widthOffset;
	this->buttons[0].xmax = 1077 * this->widthOffset;
	this->buttons[0].ymin = 453 * this->heightOffset;
	this->buttons[0].ymax = 524 * this->heightOffset;
	this->buttons[0].hover = 0;

	//Quit button
	this->buttons[1].xmin = 842 * this->widthOffset;
	this->buttons[1].xmax = 1077 * this->widthOffset;
	this->buttons[1].ymin = 554 * this->heightOffset;
	this->buttons[1].ymax = 625 * this->heightOffset;
	this->buttons[1].hover = 0;
}



//Draw method, used to display the menu
void PauseMenu::drawMenu(void) {

	//Main Window
	glPushMatrix();
	glTranslatef(743.0 * this->widthOffset, 403.0 * this->heightOffset, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, menuImage);
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
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	//Resume button
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[0].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[0]);
	}
	else if (this->buttons[0].hover == 1) {
		glBindTexture(GL_TEXTURE_2D, buttonsTexturesHover[0]);
	}
	glPushMatrix();
	glTranslatef(835.0 * this->widthOffset, 550.0 * this->heightOffset, 0.0);
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

	//Quit button
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[1].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[1]);
	}
	else if (this->buttons[1].hover == 1) {
		glBindTexture(GL_TEXTURE_2D, buttonsTexturesHover[1]);
	}
	glPushMatrix();
	glTranslatef(835.0 * this->widthOffset, 450.0 * this->heightOffset, 0.0);
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

}

//All the button textures are loaded here
void PauseMenu::loadButtons() {
	glGenTextures(1, &buttonsTextures[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/resume.bmp", &buttonsTextures[0]);
	glGenTextures(1, &buttonsTextures[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/quit.bmp", &buttonsTextures[1]);
	

	glGenTextures(1, &buttonsTexturesHover[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/resume_hover.bmp", &buttonsTexturesHover[0]);
	glGenTextures(1, &buttonsTexturesHover[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/quit_hover.bmp", &buttonsTexturesHover[1]);
}



void PauseMenu::pickButton(long x, long y)
{
	//find first polygon in which we are in bounding box
	int i;
	//For loop iterating through every button in the menu.
	for (i = 0; i < 2; i++)
	{
		//For each button, a simple BB algorithm is applied (is the cursor inside the button BB?)
		if ((y >= this->buttons[i].ymin) && (y < this->buttons[i].ymax) && (x >= this->buttons[i].xmin) && (x <= this->buttons[i].xmax))		{
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