#include "MainMenu.h"

//Class constructor where buttons bounding boxes are setup
MainMenu::MainMenu(GLfloat windowHeight, GLfloat windowWidth) : Menu(windowHeight, windowWidth) {
	
	this->buttons[0].xmin = 842 * this->widthOffset;
	this->buttons[0].xmax = 1077 * this->widthOffset;
	this->buttons[0].ymin = 600 * this->heightOffset;
	this->buttons[0].ymax = 675 * this->heightOffset;
	this->buttons[0].hover = 0;
	
	this->buttons[1].xmin = 842 * this->widthOffset;
	this->buttons[1].xmax = 1077 * this->widthOffset;
	this->buttons[1].ymin = 700 * this->heightOffset;
	this->buttons[1].ymax = 775 * this->heightOffset;
	this->buttons[1].hover = 0;

	this->buttons[2].xmin = 842 * this->widthOffset;
	this->buttons[2].xmax = 1077 * this->widthOffset;
	this->buttons[2].ymin = 800 * this->heightOffset;
	this->buttons[2].ymax = 875 * this->heightOffset;
	this->buttons[2].hover = 0;
}

//Draw method, used to display the menu
void MainMenu::drawMenu(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//Main menu bg
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

	//Buttons
	//Start button
	//If the mouse is hovering the button, a different texture will be used.
	//This is useful in order to provide a visual feedback to the player.
	//This same strategy is used for every button in the application.
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[0].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[0]);
	}
		
	else if (this->buttons[0].hover == 1) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[0]);
	}
		
	glPushMatrix();
	glTranslatef(835 * this->widthOffset, 400.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 80 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(250 * this->widthOffset, 80 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(250 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//Hi scores button
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[1].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[1]);
	}
	else if (this->buttons[1].hover == 1) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[1]);
	}
	glPushMatrix();
	glTranslatef(835 * this->widthOffset, 300.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 80 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(250 * this->widthOffset, 80 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(250 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//Quit button
	glEnable(GL_TEXTURE_2D);
	if (this->buttons[2].hover == 0) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextures[2]);
	}
	else if (this->buttons[2].hover == 1) {
		glBindTexture(GL_TEXTURE_2D, buttonsTextureHover[2]);
	}
	glPushMatrix();
	glTranslatef(835 * this->widthOffset, 200.0 * this->heightOffset, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(0.0, 80 * this->heightOffset);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(250 * this->widthOffset, 80 * this->heightOffset);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(250 * this->widthOffset, 0.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}


void MainMenu::pickButton(long x, long y)
{
	//find first polygon in which we are in bounding box
	int i;
	//For loop iterating through every button in the menu.
	for (i = 0; i < 3; i++)
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

//All the button textures are loaded here
void MainMenu::loadButtons() {
	glGenTextures(1, &buttonsTextures[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/start.bmp", &buttonsTextures[0]);
	glGenTextures(1, &buttonsTextures[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/hiscores.bmp", &buttonsTextures[1]);
	glGenTextures(1, &buttonsTextures[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/quit.bmp", &buttonsTextures[2]);
	glGenTextures(1, &buttonsTextureHover[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/start_hover.bmp", &buttonsTextureHover[0]);
	glGenTextures(1, &buttonsTextureHover[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/hiscores_hover.bmp", &buttonsTextureHover[1]);
	glGenTextures(1, &buttonsTextureHover[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures("Menu/buttons/quit_hover.bmp", &buttonsTextureHover[2]);
}