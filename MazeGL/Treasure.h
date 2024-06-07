#pragma once
#include "EventTile.h"
class Treasure :
    public EventTile
{
public:
    //Constructors and Destructor
    Treasure() {};
    Treasure(pair<GLfloat, GLfloat> coordinates, float tileBoundingSphereRadius) : EventTile(coordinates, tileBoundingSphereRadius) { this->isCollected = false; };
    ~Treasure() {};
    //Setter and Getter
    void setIsCollected(bool isCollected) { this->isCollected = isCollected; }
    bool getIsCollected() { return this->isCollected; }
private:
    //Flag to check if the treasure is collected
    bool isCollected;
};

