#pragma once
#include "Menu.h"
#include "FileManager.h"
class GameClearMenu :
    public Menu
{
public:
    //Constructors and Destructor
    GameClearMenu() {};
    GameClearMenu(FileManager, GLfloat, GLfloat);
    ~GameClearMenu() {};
    //Draw menu method used in the glutDisplayFunc callback when the game state is Game and the player won. 
    void drawMenu(void);
    //Method used in the glutPassiveMotionFunc callback.
    void pickButton(long, long);
    //Method used to load texture buttons.
    void loadButton(void);
    //Method used to save the player's score on the csv file.
    void saveToFile();

    //Setters and Getter
    void setPlayerName(string playerName) { this->playerName = playerName; }
    void setScore(int score) { this->score = score; }
    void setGameTime(int gameTime) { this->gameTime = gameTime; }
    void setDifficulty(Difficulty difficulty) { this->difficulty = difficulty; }
    int getPressedButton() { return this->pressedButton; }
   
private:    
    //button textures
    GLuint buttonTexture;
    GLuint buttonTextureHover;
    //button bounding box
    polygon button;
    //Player name
    string playerName;
    //File Manager object
    FileManager fileManager;
    //Chosen difficulty level
    Difficulty difficulty;
    //Score accumulated by the player
    int score;
    //Time the player took to exit the maze expressed in seconds
    int gameTime;
    //Index of the pressed button
    int pressedButton;

};

