#include "Menu.h"

//Constructor that accepts the window height and width as inputs.
//This will initialize the two protected variables for ease of use in the subclasses and,
//most importantly, the two offsets are initialized as well. Thery are needed as the initial UI
//was made with the standard HD resolution in mind and would break if put in any other resolution.
//This is solved by setting these offsets and by multiplying each UI component with them.
Menu::Menu(GLfloat windowHeight, GLfloat windowWidth) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->heightOffset = windowHeight / 1080.0;
	this->widthOffset = windowWidth / 1920.0;
}
//This method loads the menu image texture

void Menu::setupMenu(string textureName) {

	glClearColor(1.0, 1.0, 1.0, 0.0);

	//Loading menu pic
	glGenTextures(1, &menuImage);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	loadTextures(textureName, &menuImage);

}

//Method used to load textures
void Menu::loadTextures(string imageName, GLuint* texture) {

	int width, height, channels;
	unsigned char* img;
	img = SOIL_load_image
	(
		imageName.c_str(),
		&width, &height, &channels,
		SOIL_LOAD_AUTO
	);

	if (img != NULL) {
		switch (channels) {
		case 3: {
			glBindTexture(GL_TEXTURE_2D, *texture);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(
				GL_TEXTURE_2D, 0,
				GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, img);
			SOIL_free_image_data(img);
		}
			  break;
		case 4: {
			glBindTexture(GL_TEXTURE_2D, *texture);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(
				GL_TEXTURE_2D, 0,
				GL_RGBA, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, img);
			SOIL_free_image_data(img);
		}
		}

	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}