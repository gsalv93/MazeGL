#pragma once
#include "Includes.h"
class EventTile
{//Superclass for each special tile.
public:

	//Constructors and Destructor
	EventTile() {};
	EventTile(pair<GLfloat, GLfloat>);
	EventTile(pair<GLfloat, GLfloat>, float);
	~EventTile() {};
	//Method to check the collision with the player
	bool collisionWithPlayer(pair<GLfloat, GLfloat>, float);
	//Getter Methods
	pair<GLfloat, GLfloat> getCoordinates() { return this->coordinates; }
	float getTileBoundingSphereRadius() { return this->tileBoundingSphereRadius;}
private:
	//Coordinates of the tile inside the 3D maze
	pair<GLfloat, GLfloat> coordinates;
	//Bounding Sphere radius of the tile.
	float tileBoundingSphereRadius;
};

