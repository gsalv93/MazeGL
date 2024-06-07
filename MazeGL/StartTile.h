#pragma once
#include "EventTile.h"
class StartTile :
    public EventTile
{
public:
    //Constructors and Destructors
    StartTile() {};
    StartTile(pair<GLfloat, GLfloat> coordinates, int cardinalPosition) : EventTile(coordinates) { this->cardinalPosition = cardinalPosition; };
    ~StartTile() {};
   //Cardinal position getter.
    int getCardinalPosition() { return this->cardinalPosition; }
private:
    //Cardinal position of the tile. It's one of the following: North, South, East or West.
    int cardinalPosition;
};

