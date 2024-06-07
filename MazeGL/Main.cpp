#include "Includes.h"
#include "Game.h"
#include "MainMenu.h"
#include "HiScoresMenu.h"
#include "GameSetUpMenu.h"
#include "PauseMenu.h"
#include "GameClearMenu.h"
#include "FileManager.h"

//All glibal variables, representing the main object of the application
//Instance of Window, managed by FreeGLUT API allowing us to display the game.
Window window;
//Instance of FileManager. This object is used to manage files reading and writing when viewing or updating
//the high scores.
FileManager fileManager;
//Instance of MainMenu. This object contains information regarding the main menu of the game.
MainMenu mainMenu;
//Instance of HiScoresMenu. This object contains information regarding the high scores menu.
HiScoresMenu hiScoresMenu;
//Instance of GameSetUpMenu. This object contains information regarding the game setup menu.
GameSetUpMenu gameSetUpMenu;
//Instance of PauseMenu. This object contains information regarding the pause menu.
PauseMenu pauseMenu;
//Instance of GameClearMenu. This object contains information regarding the menu that pops up when the player wins.
GameClearMenu gameClearMenu;
//Instance of Game. This object contains information regarding the main game.
Game game;
//GameState represents an enum of all the game states. The draw callback will draw a different scene based on the state
//the game is currently in.
GameState gameState;
//Pointer to ISoundEngine isntance. This allows us to use IrrKlang library for audio implementation in the game.
ISoundEngine* engine;

//Ticks variable is used to count frames up to 60 in the timer function. This counter is needed in order
//to increase the timer displayed on the HUD of the game each second.
int ticks = 0;
int main(int argc, char **argv) {
	//If the IrrKlangDevice creation fails, the application stops.
	engine = createIrrKlangDevice();
	if (!engine)
		return 0; // error starting up the engine
	engine->setSoundVolume(0.5f);
	//Otherwise, a sound will play
	engine->play2D("Audio/Start.wav", false);
	//Each instance gets initialized with the exclusion of the Game instance, which will be initialized later in the code.
	
	//Initializing the GLUT engine
	glutInit(&argc, argv);
	window = Window();	
	//The window gets initialized
	window.windowSetup();

	mainMenu = MainMenu(window.getWindowHeight(), window.getWindowWidth());
	array<FileManager, 3> scoresFileManager;
	hiScoresMenu = HiScoresMenu(scoresFileManager, window.getWindowHeight(), window.getWindowWidth());
	gameSetUpMenu = GameSetUpMenu(window.getWindowHeight(), window.getWindowWidth());
	pauseMenu = PauseMenu(window.getWindowHeight(), window.getWindowWidth());

	fileManager = FileManager();
	gameClearMenu = GameClearMenu(fileManager, window.getWindowHeight(), window.getWindowWidth());

	
	//gameState is set to MainMenu (for obvious reasons)
	gameState = GameState::MainMenu;	

	//glutDisplayFunc contains the draw scene callback
	glutDisplayFunc(drawGameScene);
	//glutReshapeFunc contains the window resize callback
	//The reshape callback is triggered when a window is reshaped or immediately before a window's first display callback after a it is created.
	glutReshapeFunc(resize);
	glutKeyboardFunc(menusKeyboardFunc);
	//glutPassiveMotionFunc contains the mouse tracker callback.
	//The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
	glutPassiveMotionFunc(mouseTracker);
	//glutMouseFunc contains the mouse click callback
	//When a user presses and releases mouse buttons in the window, each press and each release generates a mouse callback.
	glutMouseFunc(mouseClick);
	//The window will be set to full screen
	glutFullScreen();
	//Setting main menu up, by loading the menus textures and buttons.
	mainMenu.setupMenu("Menu/mainmenu.bmp");
	mainMenu.loadButtons();
	gameSetUpMenu.setupMenu("Menu/setupmenu.bmp");
	gameSetUpMenu.loadButtons();
	hiScoresMenu.setupMenu("Menu/hiscoresmenu.bmp");
	hiScoresMenu.loadButtons();
	pauseMenu.setupMenu("Menu/pausemenu.bmp");
	pauseMenu.loadButtons();
	gameClearMenu.setupMenu("Menu/victorymenu.bmp");
	gameClearMenu.loadButton();
	
	glutMainLoop();
	//When the application exits the glut main loop, the engine is deallocated with this command
	//in order to avoid memory leaks.
	engine->drop();
	return 0;
}

//Callback function for glutDisplayFunc
//A specific scene is drawn depending on the game state.
static void drawGameScene(void) {
	switch (gameState) {
	case GameState::MainMenu: {
		mainMenu.drawMenu();
	}
	break;
	case GameState::SettingGameUp: {
		gameSetUpMenu.drawMenu();
	}
	break;
	case GameState::Game: {
		game.drawGameScene();
	}
	break;
	case GameState::HiScores: {
		hiScoresMenu.drawMenu();
	}
	break;
	
	default:
	break;
	}
}

//Callback function for glutPassiveMotionFunc (when outside the game state)
//A specific instance method is called depending on the game state.
//Each "pickButton" method checks the mouse pointer position on the screen.
void mouseTracker(int x, int y) {
	switch (gameState) {
	case GameState::MainMenu: {
		mainMenu.pickButton(x, y);
	}
	break;
	case GameState::SettingGameUp: {
		gameSetUpMenu.pickButton(x, y);
	}
	break;
	case GameState::Game: {
		//In this situation we have two possibilities:
		//Pause menu (The game is paused because the player pressed the escape key, thus opening the pause menu)
		if (game.getIsPaused() && !game.getPlayer().getHasWon())
			game.getPauseMenu().pickButton(x, y);
		//Game clear menu (The game is paused because the player won, thus opening the game clear menu)
		if (game.getPlayer().getHasWon())
			game.getGameClearMenu().pickButton(x, y);
	}
	break;
	case GameState::HiScores: {
		hiScoresMenu.pickButton(x, y);
	}
	break;
	default:
	break;
	}
}

//Callback function for glutMouseFunc.
//A specific instance method is called depending on the game state.
//Every interaction with the mouse is handled, depending on the game state.
void mouseClick(int button, int state, int x, int y){
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {//when a single left click is performed...
		//The game state is checked.
		//After checking the game state, the function looks for the pressed button.
		switch (gameState) {
		case GameState::MainMenu: {
			if (mainMenu.getPressedButton() == 0) { //Start game button is clicked
				gameState = GameState::SettingGameUp;
				glutKeyboardFunc(key);
				glutPostRedisplay();
			}
			else if (mainMenu.getPressedButton() == 1) { //Hi scores button is clicked
				gameState = GameState::HiScores;
				hiScoresMenu.getScoresFromFiles();
				glutPostRedisplay();
			}
			else if (mainMenu.getPressedButton() == 2) //Quit button is clocked
				exit(0);
		}
		break;
		case GameState::SettingGameUp: {
			//Text box is disabled each time the player clicks outside of it.
			gameSetUpMenu.setIsTextBoxEnabled(false);
			switch (gameSetUpMenu.getPressedButton()) {
				case 0://One of the three difficulty level is chosen.
					gameSetUpMenu.setDifficultyLevel(Difficulty::Easy);
					glutPostRedisplay();
				break;
				case 1:
					gameSetUpMenu.setDifficultyLevel(Difficulty::Normal);
					glutPostRedisplay();
				break;
				case 2:
					gameSetUpMenu.setDifficultyLevel(Difficulty::Hard);
					glutPostRedisplay();
				break;
				case 3://Back button is pressed, the game is set to the main menu state and every input stat gets deleted.
					gameState = GameState::MainMenu;
					glutKeyboardFunc(menusKeyboardFunc);
					resetGameStats();
					glutPostRedisplay();
				break;
				case 4://Start button is pressed.
					//The perspective projection is restored, allowing the player to navigate the maze.
					restorePerspectiveProjection();
					//The game instance is finally initialized with all the inputted information.
					game = Game(gameSetUpMenu.getPlayerName(), gameSetUpMenu.getDifficultyLevel(), gameSetUpMenu.getStyle(), pauseMenu, gameClearMenu, engine, window.getWindowHeight(), window.getWindowWidth());
					//Keyboard interaction, timer and passive motions callback are called
					setStuffForGame();
					//Finally, the game starts and the application state is put to Game state.
					game.start();
					gameState = GameState::Game;
				break;
				case 5://One of the style icons is clicked
					gameSetUpMenu.setStyle(1); //Classic icon
					glutPostRedisplay();
				break;
				case 6:
					gameSetUpMenu.setStyle(2); //Futuristic icon
					glutPostRedisplay();
				break;
				case 7:
					gameSetUpMenu.setStyle(3); //Vaporwave icon
					glutPostRedisplay();
				break;
				case 8://Textbox is clicked
					gameSetUpMenu.setIsTextBoxEnabled(true);
					glutPostRedisplay();
				break;
			}
		
		}
		break;
		
		case GameState::Game: {
			//Pause menu
			if (game.getIsPaused() && !game.getPlayer().getHasWon()) { //If the game is paused
				
				if (game.getPauseMenu().getPressedButton() == 0) { //Resume button is pressed
					//The game is resumed
					resumeGame();
					restorePerspectiveProjection();
				}
				else if (game.getPauseMenu().getPressedButton() == 1) { //Quit button is pressed
					restoringMainMenu();
				}
			}
			if (game.getPlayer().getHasWon()) { //If the game is ended
				//When the player wins, they are prompted with a window that displays their score and a single button
				//that, when pressed, brings them back to the main menu.
				if (game.getGameClearMenu().getPressedButton() == 0) {
					//The player score is saved to the file
					game.getGameClearMenu().saveToFile();
					//player won flag is set to false
					game.getPlayer().setHasWon(false);
					restoringMainMenu();
				}
			}
		}
		break;
		case GameState::HiScores: {
			if (hiScoresMenu.getPressedButton() < 3 && hiScoresMenu.getPressedButton()>-1) { //A difficulty button is pressed
				//Based on the index of the clicked button, the respective difficulty tables are shown to the player.
				int index = hiScoresMenu.getPressedButton();
				hiScoresMenu.setDifficultyStats(static_cast<Difficulty>(index));
				glutPostRedisplay();
			}
			else if (hiScoresMenu.getPressedButton() == 3) { //Back button is clicked
				gameState = GameState::MainMenu;
				glutPostRedisplay();
			}
		}
		break;
		default:
			break;
		}
	}
}

//Callback for glutKeyboardFunc.
//This function lets the player write their name on the text field.
static void key(unsigned char key, int x, int y){
	if (gameSetUpMenu.getIsTextBoxEnabled()) {
		if (key == 8) {
			if (!gameSetUpMenu.getPlayerName().empty()) {
				//If backspace is inputted, remove last character
				string buffer = gameSetUpMenu.getPlayerName();
				buffer.pop_back();
				gameSetUpMenu.setPlayerName(buffer);
			}
		}
		else if (gameSetUpMenu.getPlayerName().size() < 12) {
			//Keep adding characters until length 12 is reached.
			string buffer = gameSetUpMenu.getPlayerName();
			buffer.push_back((char)key);
			gameSetUpMenu.setPlayerName(buffer);
		}

		glutPostRedisplay();
	}
}


//Callback for glutTimerFunc.
//This funtion is used to keep calling the display function periodically
//at a rate of FPS times in one second. This will be used for animations.
static void timer(int) {
	if (game.getIsStarted()) {
		glutTimerFunc(1000 / FPS, timer, 0);
		//If the game is not paused, animate it
		if (!game.getIsPaused()) {
			glutWarpPointer(window.getWindowWidth() / 2, window.getWindowHeight() / 2); //This function makes the mouse pointer stick in the middle of the window.
			//Rotate ingame objects
			int rotationValue = game.getRotation();
			game.setRotation(rotationValue + 2);
			ticks++;
			if (ticks == 60) { //Each second the timer is increased by 1.
				ticks = 0; //ticks will be then reset to 0, starting a new count.
				int gameTime = game.getGameTime();
				gameTime += 1;
				game.setGameTime(gameTime); //the game time is updated.
				//When the player collects the key, a message is displayed for three seconds.
				//This statement manages the "timeToDisplayMessages" count down.
				if (game.getPlayer().getIsKeyObtained() && game.getTimeToDisplayMessages() > 0) {
					int timeToDisplay = game.getTimeToDisplayMessages();
					timeToDisplay -= 1;
					game.setTimeToDisplayMessages(timeToDisplay);
				}
			}
		}
		glutPostRedisplay();
	}
}


//Callback for glutPassiveMotionFunc.
//This function lets us change the yaw and pitch value with the movement of the mouse pointer.
static void inGamePassiveMotion(int x, int y) {
		// two variables to store X and Y coordinates, as observed from the center
		// of the window and not from the center of the 3D world.
		int dev_x, dev_y;
		dev_x = (window.getWindowWidth() / 2) - x;
		dev_y = (window.getWindowHeight() / 2) - y;

		//Apply the changes to pitch and yaw
		//Division by ten basically represents the mouse sensitivity, as the x and y 
		//deviations themselves are too big.
		GLfloat current_yaw = game.getPlayer().getYaw();
		GLfloat new_yaw = current_yaw + ((float)dev_x / 15.0);
		game.getPlayer().setYaw(new_yaw);
		GLfloat current_pitch = game.getPlayer().getPitch();
		GLfloat new_pitch = current_pitch + ((float)dev_y / 15.0);
		game.getPlayer().setPitch(new_pitch);
}


//Callback for glutKeyboardFunc.
//Simply put, when pressing a key used for movement, its corresponding motion will be set to true.
static void gameKeyInputDown(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'W':
	case 'w':
		game.getPlayer().setForwardMotion(true);
		break;
	case 'A':
	case 'a':
		game.getPlayer().setLeftMotion(true);
		break;
	case 'S':
	case 's':
		game.getPlayer().setBackwardMotion(true);
		break;
	case 'D':
	case 'd':
		game.getPlayer().setRightMotion(true);
		break;
	case 27: //Open the pause menu
		pauseGame();
		break;
	default:
		break;
	}
}

//This function is called when the game is put in a pause state (which happens when the player presses esc
//or when they win the game.
void pauseGame() {
	//Pause is activated, the mause is shown and unlocked from the center. This way, the player can
	//navigate the pause menu.
	game.setIsPaused(true);
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	glutPassiveMotionFunc(mouseTracker);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(gamePausedInputDown);
}

//This function is called when the game is resumed from the pause state.
void resumeGame() {
	//Pause is deactivated, the mouse is hidden from the player and locked in the middle of the screen.
	//This way, the player can start moving around the maze again.
	game.setIsPaused(false);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutPassiveMotionFunc(inGamePassiveMotion);
	glutKeyboardFunc(gameKeyInputDown);
	glutKeyboardUpFunc(gameKeyInputUp);
}

//This function is called when the game information needs to be reset.
void resetGameStats() {
	gameSetUpMenu.setDifficultyLevel(Difficulty::NoDifficultyChosen);
	gameSetUpMenu.setStyle(-1);
	gameSetUpMenu.setPlayerName("");
	gameSetUpMenu.resetButtons();
}

//Callback for glutKeyboardUpFunc.
//Simply put, when releasing a key used for movement, its corresponding motion will be set to false.
static void gameKeyInputUp(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'W':
	case 'w':
		game.getPlayer().setForwardMotion(false);
		break;
	case 'A':
	case 'a':
		game.getPlayer().setLeftMotion(false);
		break;
	case 'S':
	case 's':
		game.getPlayer().setBackwardMotion(false);
		break;
	case 'D':
	case 'd':
		game.getPlayer().setRightMotion(false);
		break;
	}
}

//Callback for glutKeyboardUpFunc.
//An empty function to disable the keyboard usage in the menus where it's not needed.
static void menusKeyboardFunc(unsigned char key, int x, int y) {
	return;
}

//Callback for glutKeyboardUpFunc.
//When the game is paused, Every button is disabled with the exception of the esc key which closes the
//pause menu.
static void gamePausedInputDown(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27: //Close the pause menu
		if (!game.getPlayer().getHasWon()) {
			resumeGame();
			restorePerspectiveProjection();
			glutPostRedisplay();
		}
		break;
	}
}


//Function used when a menu is showed on screen. Basically, an orthographic projection is applied in
//order to properly show the menu.
void applyOrthographicProjection() {
	glMatrixMode(GL_PROJECTION); //Specifies what matrix stack I'm using.
	//I copy the current projection matrix on top of the stack.
	glPushMatrix();
	//I reset this matrix to the identity
	glLoadIdentity();
	//I apply the orthographic projection transform on the identity, making the currently selected
	//projection the orthographic one.
	glOrtho(0.0, window.getWindowWidth(), 0.0, window.getWindowHeight(), 0.0, 1.0);
	//I switch back to the modelview mode.
	glMatrixMode(GL_MODELVIEW);
}

//Function used when a menu is removed from the screen and the game session is restored.
void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION); //Specifies what matrix stack I'm using.
	//I remove the projection matrix located at the top of the stack (the orthographic one).
	//This re-enables the perspective projection.
	glPopMatrix();
	//I switch back to the  modelview mode.
	glMatrixMode(GL_MODELVIEW);
}

//Resize function, where the projection matrix stack is first set up.
//The stack contains the perspective projection at the bottom (which will be used for the game)
//and the orthogonal projection at the top (which will be used for menus)
static void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Specifies what matrix stack I'm using
	glLoadIdentity();
		gluPerspective(45, (GLdouble)w / (GLdouble)h, window.getZ_near(), window.getZ_far());
	glPushMatrix();
	glLoadIdentity();
		glOrtho(0.0, w, 0.0, h, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

//Function used to restore the main menu screen from the game.
void restoringMainMenu() {
	//The player quits the game and the main menu gets restored.
	gameState = GameState::MainMenu;
	//game started flag is set to false
	game.setIsStarted(false);
	//all game setup stats are reset
	resetGameStats();
	//orthographic projection is activated, in order to see the menu
	applyOrthographicProjection();
	//lighting is disabled
	glDisable(GL_LIGHTING);
	//Set keyboard func to "menusKeyboardFunc"
	glutKeyboardFunc(menusKeyboardFunc);
	//IrrKlang engine stops all sounds.
	engine->stopAllSounds();
}

//This function contains game related glut functions, it's here just for order purposes.
void setStuffForGame() {
	//These two functions allow for keyboard interaction
	glutKeyboardFunc(gameKeyInputDown);
	glutKeyboardUpFunc(gameKeyInputUp);
	//These two functions allow for movement and animations.
	//I recall that the passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
	glutPassiveMotionFunc(inGamePassiveMotion);
	//glutTimerFunc registers the timer callback func to be triggered in at least time milliseconds (which is the first parameter).
	glutTimerFunc(1000 / FPS, timer,0);
}

//This function lets us write a text on screen.
void writeStrokeString(void* font, char* textToWrite) {
	char* c;
	for (c = textToWrite; *c != '\0'; c++)
		glutStrokeCharacter(font, *c);
}
