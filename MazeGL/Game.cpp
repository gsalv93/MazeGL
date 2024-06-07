#include "Game.h"

//Game class constructor. It initializes the player name and the difficulty level.
Game::Game(string playerName, Difficulty difficultyLevel, int style, PauseMenu pauseMenu, GameClearMenu gameClearMenu, ISoundEngine *engine, GLfloat windowHeight, GLfloat windowWidth) {
	this->playerName = playerName;
	this->difficulty = difficultyLevel;
	this->style = style;
	this->isPaused = false;
	this->pauseMenu = pauseMenu;
	this->gameClearMenu = gameClearMenu;
	this->engine = engine;
	this->rotation = 0;
	//Needed to properly set HUD position on different screen resolutions
	this->heightOffset = windowHeight / 1080.0;
	this->widthOffset = windowWidth / 1920.0;
	for (int i = 0; i < 4; i++)
		this->audioPlayed[i] = false;
}

void Game::start() {
	//Setting up the game and loading assets
	gameTime = 0;
	timeToDisplayMessages = 3; //A message will be displayed for 3 seconds. 
	//Setting up the engine
	setup();
	//Initializing the maze
	initializingMaze(this->difficulty);
	isStarted = true;
	//Setting up the player
	this->player = Player(playerName, getMaze().getStartTile().getCoordinates(), getMaze().getStartTile().getCardinalPosition());

	//I use display lists to store this static data directly on the GPU. Better performance is achieved thanks to this approach,
	//as the majority of the 3D application is static.
	//I proceed to create display lists for every block and walls.
	preparingDisplayLists();
}


void Game::setup() {
	//Initializing Lighting and Depth Buffer in order to avoid overlapping between objects and walls.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	//Hiding the mouse cursor with the following function.
	glutSetCursor(GLUT_CURSOR_NONE);
	//Specifying the red, green, blue, and alpha values used by glClear to clear the color buffers.
	glClearColor(1.0, 1.0, 1.0, 0.0);

	//Loading textures based on the chosen style
	loadingTextures();

	//Finally, loading the two 3D models
	keyModel.load("Models/", "key.obj", "key.mtl");
	diamondModel.load("Models/", "diamond.obj", "diamond.mtl");

}

//This method simply loads texture data in.
void Game::loadingTextures() {
	string path = "Textures/style";
	path.append(to_string(style));

	glGenTextures(1, &mazeTextures[0]);
	loadTextures(path + "/wall.bmp", &mazeTextures[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &mazeTextures[1]);
	loadTextures(path + "/floor.bmp", &mazeTextures[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &mazeTextures[2]);
	loadTextures(path + "/ceiling.bmp", &mazeTextures[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//Loading HUD textures
	glGenTextures(1, &hudElements[0]);
	loadTextures("HUD/hud.bmp", &hudElements[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &hudElements[1]);
	loadTextures("HUD/exit_error.bmp", &hudElements[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &hudElements[2]);
	loadTextures("HUD/key_found.bmp", &hudElements[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &hudIcons[0]);
	loadTextures("HUD/key.png", &hudIcons[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &hudIcons[1]);
	loadTextures("HUD/crystal.png", &hudIcons[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//Loading start and finish textures
	glGenTextures(1, &startTexture);
	loadTextures("Textures/start.png", &startTexture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &finishTexture);
	loadTextures("Textures/smiley.png", &finishTexture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

//This method prepares the display lists for static objects in the maze.
void Game::preparingDisplayLists() {
	//Display list for block walls
	walls = glGenLists(1);
	glNewList(walls, GL_COMPILE);
	glPushMatrix();
	for (MazeBlockWall& mazeBlockWall : getMaze().getMazeBlocks()) {
		//Drawing the block
		glEnable(GL_TEXTURE_2D);
		//I activate backface culling in order not to draw the back faces of each cube, thus reducing
		//strain on the GPU
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBindTexture(GL_TEXTURE_2D, mazeTextures[0]);
		mazeBlockWall.drawBlock();
		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
	glEndList();

	//Display list for the external walls, floor and ceiling included
	externalWalls = glGenLists(2);
	glNewList(externalWalls, GL_COMPILE);
	glPushMatrix();
	//Drawing the floor
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mazeTextures[1]);
	glPushMatrix();
	getMaze().getFloor().draw();
	glPopMatrix();

	//Drawing the ceiling
	glBindTexture(GL_TEXTURE_2D, mazeTextures[2]);
	glPushMatrix();
	glTranslatef(0.0, 10.0, 0.0);
	getMaze().getCeiling().draw();
	glPopMatrix();

	//Drawing the external walls
	glBindTexture(GL_TEXTURE_2D, mazeTextures[0]);
	for (int i = 0; i < 4; i++) {
		getMaze().getWalls().at(i).draw();
		getMaze().getWalls().at(i).calcCoordinates();
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEndList();

	//Display list for the entrance
	entry = glGenLists(3);
	glNewList(entry, GL_COMPILE);
	glPushMatrix();
	//This is where the player is located when starting the game.
	glEnable(GL_TEXTURE_2D);
	//This blend function has to be enabled as the imported texture contains an alpha channel.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, startTexture);
	//Drawing the sign
	glPushMatrix();
	GLfloat startXCoord = getMaze().getStartTile().getCoordinates().first;
	GLfloat startZCoord = getMaze().getStartTile().getCoordinates().second;

	switch (getMaze().getStartTile().getCardinalPosition()) { //Get cardinal position of the starting tile, so that I can
	case 0:													  //rotate the START sign properly.
		//North
		glTranslatef(startXCoord, 0.0, -(startZCoord - 3.5));
		glRotatef(90, 0, 1, 0);
		break;
	case 1:
		//East
		glTranslatef(startXCoord - 3.5, 0.0, -startZCoord);
		break;
	case 2:
		//South
		glTranslatef(startXCoord, 0.0, -(startZCoord + 3.5));
		glRotatef(270, 0, 1, 0);
		break;
	case 3:
		//West
		glTranslatef(startXCoord + 3.5, 0.0, -startZCoord);
		glRotatef(180, 0, 1, 0);
		break;
	}
	//Finally, drawing the quad itself.
	glScalef(0.015, 0.015, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0, 128, 0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(509, 128, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(509, 0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
	glEndList();


	//Display list for the exit
	exit = glGenLists(4);
	glNewList(exit, GL_COMPILE);
	glPushMatrix();
	GLfloat exitXCoord = getMaze().getExitTile().getCoordinates().first;
	GLfloat exitZCoord = getMaze().getExitTile().getCoordinates().second;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, finishTexture);
	glPushMatrix();
	switch (getMaze().getExitTile().getCardinalPosition()) { //Get cardinal position of the starting tile, so that I can
	case 0:												 //rotate the exit smiley properly.
		//North
		glTranslatef(exitXCoord, -2.5, -(exitZCoord - 3));
		glRotatef(90, 0, 1, 0);
		break;
	case 1:
		//East
		glTranslatef(exitXCoord - 3, -2.5, -exitZCoord);
		break;
	case 2:
		//South
		glTranslatef(exitXCoord, -2.5, -(exitZCoord + 3));
		glRotatef(270, 0, 1, 0);
		break;
	case 3:
		//West
		glTranslatef(exitXCoord + 3, -2.5, -exitZCoord);
		glRotatef(180, 0, 1, 0);
		break;
	}

	glScalef(0.05, 0.05, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0, 126, 0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(126, 126, 0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(126, 0, 0);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
	glEndList();
}

//This method initializes the maze, starting from the difficulty chosen by the player.
//The difficulty will change the dimension of the maze.
//I'm also setting the bonus threshold which is used at the end of the game to check if
//the player is awarded with some bonus points.
void Game::initializingMaze(Difficulty difficulty) {
	int maze_size = 0;
	switch (difficulty) {
	case Difficulty::Easy:
		maze_size = 7;
		bonusThreshold = 60;
		break;
	case Difficulty::Normal:
		maze_size = 9;
		bonusThreshold = 90;
		break;
	case Difficulty::Hard:
		maze_size = 11;
		bonusThreshold = 120;
		break;
	}

	maze = Maze(maze_size, maze_size);
	getMaze().generate();

}

//Main drawn scene of the application.
void Game::drawGameScene(void) {
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//HUD is drawn
	drawHUD();

	glLoadIdentity();
	//Camera method is called since everytime we move, the scene has to be updated with the new perspective.
	camera();

	// ***DRAWING STUFF***

	//Drawing the floor, the ceiling and the four external walls.
	glCallList(externalWalls);
	//Checking collision between player and wall
	for (int i = 0; i < 4; i++) {
		getPlayer().setIsCollided(getMaze().getWalls().at(i).checkCollisionWithPlayer(getPlayer().getCoordinates()));
		if (getPlayer().getIsCollided()) {
			managePlayerCollisionWithWall();
		}
	}
	//Drawing the maze itself
	glCallList(walls);
	//Checking for collision with a block
	for (MazeBlockWall& mazeBlockWall : getMaze().getMazeBlocks()) {
		getPlayer().setIsCollided(mazeBlockWall.checkCollisionWithPlayer(getPlayer().getCoordinates(), getPlayer().getBoundingSphereRadius()));
		if (getPlayer().getIsCollided())
			//If a collision happens, the player is pushed out of the wall.
			managePlayerCollisionWithWall();
	}

	//Drawing the start sign
	glCallList(entry);

	//Drawing the key
	//If the key hasn't been collected yet, it is drawn.
	if (!getPlayer().getIsKeyObtained()) {
		GLfloat keyXCoord = this->getMaze().getKey().getCoordinates().first;
		GLfloat keyZCoord = this->getMaze().getKey().getCoordinates().second;
		glEnable(GL_TEXTURE_2D);
		//I, again, activate backface culling in order not to draw the back faces of each cube, thus reducing
		//strain on the GPU
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPushMatrix();
			glTranslatef(keyXCoord, 0, -(keyZCoord));
			glScalef(0.05, 0.05, 0.05);
			glRotatef(this->rotation, 0, 1, 0);
			keyModel.render();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		//If the player collides with the key, the player will collect it and a sound effect will play.
		if (getMaze().getKey().collisionWithPlayer(getPlayer().getCoordinates(), getPlayer().getBoundingSphereRadius())) {
			getPlayer().setIsKeyObtained(true);
			if (!audioPlayed[0]) {
				audioPlayed[0] = true; //I set to true the audioPlayed check so that it won't get played in the future.
				engine->play2D("Audio/CHIMES.wav", false);
			}
			//Once the key is obtained, the player can escape the maze.
			getMaze().getExitTile().setIsUnlocked(true);
		}
	}

	//Drawing the treasures
	//I iterate through the treasures map in the Maze class.
	for (auto& treasure : getMaze().getTreasures()) {
		//If the treasure hasn't been collected yet, I draw it.
		if (!treasure.second.getIsCollected()) {
			GLfloat treasureXCoord = treasure.second.getCoordinates().first;
			GLfloat treasureZCoord = treasure.second.getCoordinates().second;
			glEnable(GL_TEXTURE_2D);
			//I, again, activate backface culling in order not to draw the back faces of each cube, thus reducing
			//strain on the GPU
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glPushMatrix();
				glTranslatef(treasureXCoord, 2.5, -(treasureZCoord));
				glRotatef(180, 1, 0, 0);
				glScalef(0.10, 0.10, 0.10);
				glRotatef(this->rotation, 0, 1, 0);
				diamondModel.render();
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_CULL_FACE);
			//If the player collides with the treasure, the player will collect it and a sound effect will play.
			if (treasure.second.collisionWithPlayer(getPlayer().getCoordinates(), getPlayer().getBoundingSphereRadius())) {
				if (!audioPlayed[1]) {
					audioPlayed[1] = true;
					engine->play2D("Audio/DING.wav", false);
				}
				int currTreasures = getPlayer().getNumberOfCollectedTreasures();
				getPlayer().setNumberOfCollectedTreasures(currTreasures + 1);
				int currScore = getPlayer().getScore();
				getPlayer().setScore(500 + currScore);
				treasure.second.setIsCollected(true);
			}
			else//If the player is out of the treasure's BB, I set to false the audioPlayed check so that this specific
				//sound effect may be played in the future
				audioPlayed[1] = false;
		}

	}
	
	//Drawing Exit Tile
	//Pretty similar to the start tile. This is the exit tile where the player can leave the
	//maze, after collecting the key, however.
	glCallList(exit);
	

	//Checking collision between the player and the exit tile. If the player collides, another check is performed.
	//This other check is about the possession of the key to leave the maze.
	if (getMaze().getExitTile().collisionWithPlayer(getPlayer().getCoordinates(), getPlayer().getBoundingSphereRadius())) {
		//Check if the player collected the key.
		if (getMaze().getExitTile().getIsUnlocked()) {
			//In this situation, the player won the game.
			//A sound effect is played.
			if (!audioPlayed[3]) {
				audioPlayed[3] = true;
				engine->play2D("Audio/TADA.wav", false);
			}

			//The player is allowed to leave the maze, thus ending the game.
			//Show screen with time and points. Also, gotta convert time to points
			timeToDisplayMessages = 0;

			//Calculating bonus points if the player completed the maze swiftly.
			//If the player was faster than a certain threshold, they will be awarded extra points.
			int bonusPoints = 0;
			if (gameTime < bonusThreshold) //The faster the player, the greater the bonus points.
				bonusPoints = (bonusThreshold - gameTime) * 10;
			//Setting the flag that the player has won the game.
			getPlayer().setHasWon(true);
			//Passing player data to the game clear menu
			gameClearMenu.setPlayerName(getPlayer().getName());
			gameClearMenu.setGameTime(gameTime);
			gameClearMenu.setScore(getPlayer().getScore() + bonusPoints);
			gameClearMenu.setDifficulty(this->difficulty);
			//Pausing the game, preventing the clock from counting seconds after the end of the game and so on.
			pauseGame();
		}
		else {
			//If the player hasn't collected the key yet, a sound effect is played.
			if (!audioPlayed[2]) {
				audioPlayed[2] = true;
				engine->play2D("Audio/CHORD.wav", false);
			}
		}
	}
	else {//When the player leaves the exit BB, I set to false the audioPlayed check so that this specific
				//sound effect may be played in the future
		audioPlayed[2] = false;
	}
	glutSwapBuffers();
}

//This method is used to draw the HUD.
void Game::drawHUD() {
	//I have to disable lighting in order to properly draw the HUD.
	//Lighting will be restored right after drawing the HUD.

	glDisable(GL_LIGHTING);
	//Swap to orthographic
	applyOrthographicProjection();
	glLoadIdentity();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//draw HUD

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, hudElements[0]);
	glPushMatrix();
		glTranslatef(0, 805.0 * this->heightOffset, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(0.0, 0.0);

			glTexCoord2f(0.0, 1.0);
			glVertex2f(0.0, 275.0 * this->heightOffset);

			glTexCoord2f(1.0, 1.0);
			glVertex2f(435.0 * this->widthOffset, 275.0 * this->heightOffset);

			glTexCoord2f(1.0, 0.0);
			glVertex2f(435.0 * this->widthOffset, 0.0);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//Writing text on screen
	//Time
	glPushMatrix();
		//If the player takes too much time, they will no longer awarded bonus points.
		//This will be signaled in the game by changing the time text color to red.
		if (gameTime <= bonusThreshold)
			glColor3f(0.0, 0.0, 0.0);
		else
			glColor3f(1.0, 0.0, 0.0);
		glTranslatef(10.0 * this->widthOffset, 985.0 * this->heightOffset, 0);
		glScalef(0.25 * this->widthOffset, 0.25 * this->heightOffset, 1);
		string text = "TIME: " + to_string(gameTime);
		writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)text.c_str());
	glPopMatrix();

	//Treasures
	glEnable(GL_TEXTURE_2D);
	//By applying this blending function, I'm able to remove the black background from the imported texture
	//that has an alpha channel
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, hudIcons[1]);
	glPushMatrix();
		
		glTranslatef(10.0 * this->widthOffset, 850.0 * this->heightOffset, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(0.0, 0.0);

		glTexCoord2f(1.0, 0.0);
		glVertex2f(0.0, 48.0 * this->heightOffset);

		glTexCoord2f(0.0, 0.0);
		glVertex2f(48.0 * this->widthOffset, 48.0 * this->heightOffset);

		glTexCoord2f(0.0, 1.0);
		glVertex2f(48.0 * this->widthOffset, 0.0);
		glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	//Text near the treasure icon
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(70.0 * this->widthOffset, 850.0 * this->heightOffset, 0);
		glScalef(0.15 * this->widthOffset, 0.15 * this->heightOffset, 1);
		char number = static_cast<char>(getPlayer().getNumberOfCollectedTreasures());
		text = "x " + to_string(number);

		writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char *)text.c_str());
	glPopMatrix();

	//Key
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, hudIcons[0]);
	glPushMatrix();
		glTranslatef(250.0 * this->widthOffset, 850.0 * this->heightOffset, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(0.0, 0.0);

		glTexCoord2f(1.0, 0.0);
		glVertex2f(0.0, 48.0 * this->heightOffset);

		glTexCoord2f(0.0, 0.0);
		glVertex2f(48.0 * this->widthOffset, 48.0 * this->heightOffset);

		glTexCoord2f(0.0, 1.0);
		glVertex2f(48.0 * this->widthOffset, 0.0);
		glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	//Text near the key icon
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(310.0 * this->widthOffset, 850.0 * this->heightOffset, 0);
		glScalef(0.15 * this->widthOffset, 0.15 * this->heightOffset, 1);
		number = static_cast<char>(getPlayer().getIsKeyObtained());
		text = "x " + to_string(number);
		writeStrokeString(GLUT_STROKE_MONO_ROMAN, (char*)text.c_str());
	glPopMatrix();

	//If the player has collected the key, a message is displayed for timeToDisplayMessages seconds.
	if (getPlayer().getIsKeyObtained() && timeToDisplayMessages > 0) {
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hudElements[2]);
		glPushMatrix();
		glTranslatef(845.0 * this->widthOffset, 640.0 * this->heightOffset, 0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);
				glVertex2f(0.0, 0.0);

				glTexCoord2f(0.0, 1.0);
				glVertex2f(0.0, 155.0 * this->heightOffset);

				glTexCoord2f(1.0, 1.0);
				glVertex2f(230.0 * this->widthOffset, 155.0 * this->heightOffset);

				glTexCoord2f(1.0, 0.0);
				glVertex2f(230.0 * this->widthOffset, 0.0);
			glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	//If the player hasn't collected the key yet and they're on the exit tile
	if (getMaze().getExitTile().collisionWithPlayer(getPlayer().getCoordinates(), getPlayer().getBoundingSphereRadius()) && !getMaze().getExitTile().getIsUnlocked()) {

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, hudElements[1]);
		glPushMatrix();
			glTranslatef(845.0 * this->widthOffset, 640.0 * this->heightOffset, 0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);
				glVertex2f(0.0, 0.0);

				glTexCoord2f(0.0, 1.0);
				glVertex2f(0.0, 155.0 * this->heightOffset);

				glTexCoord2f(1.0, 1.0);
				glVertex2f(230.0 * this->widthOffset, 155.0 * this->heightOffset);

				glTexCoord2f(1.0, 0.0);
				glVertex2f(230.0 * this->widthOffset, 0.0);
			glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	if (isPaused) {//If the game is paused, draw the pause menu
		this->pauseMenu.drawMenu();
	}

	if (getPlayer().getHasWon()) {//If  the game is completed, draw the game clear menu
		this->gameClearMenu.drawMenu();
	}

	//Get back to perspective projection and draw the rest of the game
	restorePerspectiveProjection();
	//Restore lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

//This function lets us move around in the 3D world.
void Game::camera(){
	//When one of the four keyboard keys are pressed, its corresponding "motion" value becomes true. (button presses are managed in the keyboard buttons callback)
	//This lets the player move in the world.
	//Actually though, it's the 3D world itself that rotates and traslates around the camera.

	//These checks are pretty straight forward: if a button is pressed, move in that direction.
	//Movement is achieved through trigonometry!
	//When pointing to a direction, the camera makes an angle between the Z axis and the direction it's looking at. This angle is the yaw.
	//When calculating movement, we need the angle formed between the camera direction and the X axis: that's yaw+90 (for forward movement).
	//We can now update the camX and camZ value thanks to the cosine and sine functions, respectively. camZ's value is subtracted as the Z axis
	//is negative.
	//Finally, the division is there in order to slow the movement down. By tweaking with it we can choose how fast the player moves.
	
	GLfloat player_x_coord = get<0>(getPlayer().getCoordinates());
	GLfloat player_z_coord = get<1>(getPlayer().getCoordinates());

	if (getPlayer().getForwardMotion()){
		player_x_coord += cos((getPlayer().getYaw() + 90) * TO_RADIANS) / 2;
		player_z_coord -= sin((getPlayer().getYaw() + 90) * TO_RADIANS) / 2;

	}
	if (getPlayer().getBackwardMotion()){
		player_x_coord += cos((getPlayer().getYaw() + 90 + 180) * TO_RADIANS) / 2;
		player_z_coord -= sin((getPlayer().getYaw() + 90 + 180) * TO_RADIANS) / 2;

	}
	if (getPlayer().getLeftMotion()){
		player_x_coord += cos((getPlayer().getYaw() + 90 + 90) * TO_RADIANS) / 2;
		player_z_coord -= sin((getPlayer().getYaw() + 90 + 90) * TO_RADIANS) / 2;

	}
	if (getPlayer().getRightMotion()){
		player_x_coord += cos((getPlayer().getYaw() + 90 - 90) * TO_RADIANS) / 2;
		player_z_coord -= sin((getPlayer().getYaw() + 90 - 90) * TO_RADIANS) / 2;

	}
	
		getPlayer().setCoordinates(player_x_coord, player_z_coord);

		//limit the values of pitch between -25 and 25.
		if (getPlayer().getPitch() >= 25)
			getPlayer().setPitch(25);
		if (getPlayer().getPitch() <= -25)
			getPlayer().setPitch(-25);
		
		//Rotations and Translation. The order is mandatory!
		//A double rotation with the new pitch and yaw values will be applied,
		//along with a translation with the new values of the player position.
		glRotatef(-getPlayer().getPitch(), 1.0, 0.0, 0.0);  //Along X axis
		glRotatef(-getPlayer().getYaw(), 0.0, 1.0, 0.0);    //Along Y axis

		//The translation is negative as the camera itself is not moving, but it's the 3D world around it that gets
		//translated towards the origin. (that's also why we used the negative values of the angle when rotating)
		glTranslatef(-player_x_coord, 0.0, -player_z_coord);
}

//If a collision happens, the player is pushed out of the wall.
void Game::managePlayerCollisionWithWall() {
	GLfloat player_x_coord = get<0>(getPlayer().getCoordinates());
	GLfloat player_z_coord = get<1>(getPlayer().getCoordinates());
	if (getPlayer().getForwardMotion())
	{
		player_x_coord += cos((getPlayer().getYaw() + 90 + 180) * TO_RADIANS) / 2;
		player_z_coord -= sin((getPlayer().getYaw() + 90 + 180) * TO_RADIANS) / 2;
	}
	if (getPlayer().getBackwardMotion())
	{
		player_x_coord += cos((getPlayer().getYaw() + 90) * TO_RADIANS) / 2;
		player_z_coord -= sin((getPlayer().getYaw() + 90) * TO_RADIANS) / 2;

	}
	if (getPlayer().getLeftMotion())
	{
		player_x_coord += cos((getPlayer().getYaw() + 90 - 90) * TO_RADIANS) / 2;
		player_z_coord -= sin((getPlayer().getYaw() + 90 - 90) * TO_RADIANS) / 2;
	}
	if (getPlayer().getRightMotion())
	{
		player_x_coord += cos((getPlayer().getYaw() + 90 + 90) * TO_RADIANS) / 2;
		player_z_coord -= sin((getPlayer().getYaw() + 90 + 90) * TO_RADIANS) / 2;

	}

	getPlayer().setCoordinates(player_x_coord, player_z_coord);
}

//Function used to load textures
void Game::loadTextures(string imageName, GLuint* texture) {

	int width, height, channels;
	unsigned char* img;
	img = SOIL_load_image
	(
		imageName.c_str(),
		&width, &height, &channels,
		SOIL_LOAD_AUTO
	);

	if (img != NULL) {
		switch (channels) {
			case 3: {
				glBindTexture(GL_TEXTURE_2D, *texture);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(
					GL_TEXTURE_2D, 0,
					GL_RGB, width, height, 0,
					GL_RGB, GL_UNSIGNED_BYTE, img);
				SOIL_free_image_data(img);
			}
			break;
			case 4: {
				glBindTexture(GL_TEXTURE_2D, *texture);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(
					GL_TEXTURE_2D, 0,
					GL_RGBA, width, height, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, img);
				SOIL_free_image_data(img);
			}
		}
		
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
