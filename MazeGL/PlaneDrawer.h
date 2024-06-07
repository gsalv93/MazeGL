#pragma once
#include "Includes.h"
class PlaneDrawer
{
public:
	//Constructor and Destructor
	PlaneDrawer() {};
	PlaneDrawer(GLfloat length, GLfloat height, GLfloat width) {
		this->height = height;
		this->length = length;
		this->width = width;
	};
	~PlaneDrawer() {};
	//Method to simply draw a plane
	void draw();
	//Setter and Getters Methods
	GLfloat getHeight() { return this->height; }
	GLfloat getLength() { return this->length; }
	GLfloat getWidth() { return this->width; }
	tuple<GLfloat, GLfloat, GLfloat>getCoordinates() { return this->coordinates; }
	void setCoordinates(GLfloat x, GLfloat y, GLfloat z){ this->coordinates = make_tuple(x, y, z); }	
private:
	//3D Coordinates of the plane
	tuple<GLfloat, GLfloat, GLfloat> coordinates;
	//Its height, length and width.
	GLfloat height;
	GLfloat length;
	GLfloat width;
};

