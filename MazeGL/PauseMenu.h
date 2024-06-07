#pragma once
#include "Menu.h"
class PauseMenu :
    public Menu
{
public:
    //Constructor and Destructor
    PauseMenu() {};
    PauseMenu(GLfloat, GLfloat);
    ~PauseMenu() {};
    //Draw menu method used in the glutDisplayFunc callback when the game state is Game and the player opened the pause menu. 
    void drawMenu(void);
    //Method used in the glutPassiveMotionFunc callback.
    void pickButton(long, long);
    //Method used to load texture buttons.
    void loadButtons(void);
    //Getter method to retrieve the pressed button.
    int getPressedButton() { return this->pressedButton; }
private:
    //Buttons textures and BB.
    GLuint buttonsTextures[2];
    GLuint buttonsTexturesHover[2];
    polygon buttons[2];
    //index of pressed button.
    int pressedButton;
};

