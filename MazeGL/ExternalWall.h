#pragma once
#include "PlaneDrawer.h"
class ExternalWall :
    public PlaneDrawer
{
public:
    //Constructors and Destructor
    ExternalWall() {};
    ExternalWall(GLfloat length, GLfloat height, GLfloat width, int cardinalPosition) : PlaneDrawer(length, height, width) { this->cardinalPosition = cardinalPosition; };
    ~ExternalWall() {};
    //Method that overrides the draw function of PlaneDrawer.
    void draw();
    //Method used to check the collision with the player.
    bool checkCollisionWithPlayer(tuple<GLfloat, GLfloat>);
    //This method calculates the wall coordinates based on its cardinal position.
    void calcCoordinates();
    //Getter method to obtain the cardinalPosition (again, North, South, West or East).
    int getCardinalPosition() { return this->cardinalPosition; }
private:
    //Cardinal position of the wall.
    int cardinalPosition;
    void drawNorthWall();
    void drawEastWall();
    void drawSouthWall();
    void drawWestWall();
};

