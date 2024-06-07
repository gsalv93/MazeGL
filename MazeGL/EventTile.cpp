#include "EventTile.h"

//Constructor that gets overridden by every special tile subclass.
EventTile::EventTile(pair<GLfloat, GLfloat> coordinates, float tileBoundingSphereRadius) {
	this->coordinates = coordinates;
	this->tileBoundingSphereRadius = tileBoundingSphereRadius;
}
//Constructor that gets overridden by the start tile. (as it lacks a bounding box since we don't care about it)
EventTile::EventTile(pair<GLfloat, GLfloat> coordinates) {
	this->coordinates = coordinates;
	this->tileBoundingSphereRadius = 0.0;
}

//Generic collision detection between with two spheres.
bool EventTile::collisionWithPlayer(pair<GLfloat, GLfloat> playerCoordinates, float playerBB) {
	
	//Calculating the distance between the two spheres
	GLfloat xPlayerCoord = playerCoordinates.first;
	GLfloat zPlayerCoord = playerCoordinates.second;
	
	float sphereXDistance = abs(xPlayerCoord - (this->coordinates.first));
	float sphereZDistance = abs(zPlayerCoord + (this->coordinates.second)); //This gets negated since the z coordinate is always negative

	if (sphereXDistance >= (tileBoundingSphereRadius + playerBB))
		return false;
	if (sphereZDistance >= (tileBoundingSphereRadius + playerBB))
		return false;

	return true;
}