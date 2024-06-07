#pragma once
#include "EventTile.h"
class ExitTile :
    public EventTile
{
public:
    //Constructors and Destructors
    ExitTile() {};
    ExitTile(pair<GLfloat, GLfloat> coordinates, float tileBoundingSphereRadius, int cardinalPosition) : EventTile(coordinates, tileBoundingSphereRadius) { this->isUnlocked = false; this->cardinalPosition = cardinalPosition; };
    ~ExitTile() {};
    //Getters and Setter methods
    bool getIsUnlocked() { return this->isUnlocked; }
    int getCardinalPosition() { return this->cardinalPosition; }
    void setIsUnlocked(bool isUnlocked) { this->isUnlocked = isUnlocked; }
private:
    //Cardinal position of the tile. It's one of the following: North, South, East or West.
    int cardinalPosition;
    //Flag to check if the exit door is unlocked
    bool isUnlocked;
};

