#pragma once
#include "EventTile.h"

class Key :
    public EventTile
{
public://Constructors and Destructor
    Key() { };
    Key(pair<GLfloat, GLfloat> coordinates, float tileBoundingSphereRadius) : EventTile(coordinates, tileBoundingSphereRadius) {};
    ~Key() {};
private:    
};

