#include "Maze.h"


Maze::Maze(int w, int h) {
	visitedCells = 1;
	//the size gets doubled as a wall tile is as big as walkable tile. This means that the maze in output has to be twice as big.
	mazeWidth = w * 2;
	mazeHeight = h * 2;
	//Number of treasures scattered in the maze.
	numberOfTreasures = 3;
	//asciiMaze is the char 2D array that will contain the final maze.
	//The final +1 takes into account the terminate character.
	asciiMaze = new char[((mazeWidth + 1) * (mazeHeight + 1)) + 1]();
	//The ascii maze is initialized with all @ character, which represent a walkable tile.
	memset(asciiMaze, '@', ((mazeWidth + 1) * (mazeHeight + 1)));
}


bool Maze::generate() {
	//starting a seed for pseudo-random number generation
	srand(time(NULL));

	//In this method a temporary maze will be created, which will be half the size of the one that will be displayed.
	//This decision was made as the maze built with this code is drawn with a proprietary engine in mind that allows for
	//maze walls to be lines. that's different from the proposed approach as a maze wall is basically a block.
	//(more info about the maze can be found in the github shown in Maze.h)
	int m_nMazeWidth = mazeWidth / 2;
	int m_nMazeHeight = mazeHeight / 2;
	int* m_maze = new int[m_nMazeWidth * m_nMazeHeight]();
	memset(m_maze, 0x00, m_nMazeWidth * m_nMazeHeight * sizeof(int));
	// Choose a starting cell from where the generation can begin
	int x = rand() % m_nMazeWidth;
	int y = rand() % m_nMazeHeight;
	m_stack.push(make_pair(x, y)); //BEGINNING POINT
	m_maze[y * m_nMazeWidth + x] = CELL_VISITED; //This cell is set as visited
	asciiMaze[(y * 2) * (mazeWidth + 1) + (x * 2)] = '#'; //the corresponding ascii maze block is set as a wall

	//Maze exploration can now begin. This way, the maze is generated, along with the ascii maze.
	mazeExploration(m_maze, m_nMazeWidth, m_nMazeHeight);
	
	//After this method we finally have our maze! The following steps let us find the...
	//Start tile of the player.
	//Exit tile that the player will have to reach with a key in order to win.
	//tile containing a key.
	//tiles containing a treasure.
	int end_position = rand() % 4;//0 -> 2 | 2 -> 0 | 1 -> 3 | 3 -> 1
	int start_position = (2 + end_position) % 4; //Direzione opposta a quella iniziale
	
	
	generateStartDoor(start_position);
	
	generateExitDoor(end_position);
	
	specialTiles();
	setupPlanes();
	build3DMaze();
	print();
	//Freeing memory
	delete[] m_maze;
	delete[] asciiMaze;
	return true;
}

//This method creates a vector of MazeBlockWall objects. Basically, it contains every cube that represents a wall inside the maze.
void Maze::build3DMaze() {
	int w = mazeWidth;
	int h = mazeHeight;
	//I iterate through the Ascii Maze and, for each '#' character, I add a block to the vector, containing the 3D coordinates of its location in the maze.
	for (int i = 0; i < h - 1; i++) {
		for (int j = 0; j < w - 1; j++) {
			GLfloat coordx = 10 * (w - i);
			GLfloat coordz = 10 * (h - j);
			char mazeTile = getAsciiMaze()[j * (w + 1) + i];
			if (mazeTile == '#') {
				mazeBlocks.push_back(MazeBlockWall(coordx, 0, -coordz));
			}
		}
	}
}


//This is where the maze is generated thanks to the recursive backtrack approach.
void Maze::mazeExploration(int* m_maze, int m_nMazeWidth, int m_nMazeHeight){
	while (visitedCells < (m_nMazeWidth * m_nMazeHeight)){
		// Create a set of unvisted neighbours
		vector<int> neighbours;
		// Checking neighbours of the cell at the top of the stack.
		// Each if consists in checking if I'm going out of bounds when checking for a neighbour and if that neighbour has been visited.
		// If I don't go out of bounds and that neighbour is not visited, then I add that cell to the neighbours vector.
		// This check is applied for each cardinal direction.
		// North neighbour
		if (m_stack.top().second > 0 && (m_maze[offset(0, -1)] & CELL_VISITED) == 0)
			neighbours.push_back(0);
		// East neighbour
		if (m_stack.top().first < m_nMazeWidth - 1 && (m_maze[offset(1, 0)] & CELL_VISITED) == 0)
			neighbours.push_back(1);
		// South neighbour
		if (m_stack.top().second < m_nMazeHeight - 1 && (m_maze[offset(0, 1)] & CELL_VISITED) == 0)
			neighbours.push_back(2);
		// West neighbour
		if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
			neighbours.push_back(3);

		// Are there any neighbours available?
		if (!neighbours.empty()){
			// Choose one available neighbour at random
			int next_cell_dir = neighbours[rand() % neighbours.size()];

			// Create a path between the neighbour and the current cell
			// When creating this path, I make sure to add two wall tiles to the ascii maze.
			switch (next_cell_dir){
			case 0: // North
				//Path between the two tiles is created
				m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
				m_maze[offset(0, 0)] |= CELL_PATH_N;
				//and then pushed into the stack
				m_stack.push(make_pair((m_stack.top().first + 0), (m_stack.top().second - 1)));
				//Creating the path for the ascii maze
				//I first check if I'm going out of bounds.
				if (int(ascii_offset(0, -1)) >= 0) {
					asciiMaze[int(ascii_offset(0, -1))] = '#';
					
				}
					
				if (int(ascii_offset(0, -.5)) >= 0) {
					asciiMaze[int(ascii_offset(0, -.5))] = '#';
					
				}
				break;
				//This idea is applied for every other cardinal direction.
			case 1: // East
				m_maze[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
				m_maze[offset(0, 0)] |= CELL_PATH_E;
				m_stack.push(make_pair((m_stack.top().first + 1), (m_stack.top().second + 0)));
				if (int(ascii_offset(1, 0)) >= 0) {
					asciiMaze[int(ascii_offset(1, 0))] = '#';
					
				}
				if (int(ascii_offset(.5, 0)) >= 0) {
					asciiMaze[int(ascii_offset(.5, 0))] = '#';
					
				}
				break;

			case 2: // South
				m_maze[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
				m_maze[offset(0, 0)] |= CELL_PATH_S;
				m_stack.push(make_pair((m_stack.top().first + 0), (m_stack.top().second + 1)));
				if (int(ascii_offset(0, 1)) >= 0) {
					asciiMaze[int(ascii_offset(0, 1))] = '#';
					
				}
				if (int(ascii_offset(0, .5)) >= 0) {
					asciiMaze[int(ascii_offset(0, .5))] = '#';
					
				}
				break;

			case 3: // West 
				m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
				m_maze[offset(0, 0)] |= CELL_PATH_W;
				m_stack.push(make_pair((m_stack.top().first - 1), (m_stack.top().second + 0)));
				if (int(ascii_offset(-1, 0)) >= 0) {
					asciiMaze[int(ascii_offset(-1, 0))] = '#';
					
				}
				if (int(ascii_offset(-.5, 0)) >= 0) {
					asciiMaze[int(ascii_offset(-.5, 0))] = '#';
					
				}
				break;

			}
			//Finally, I add a wall to the current tile.
			if (int(ascii_offset(0, 0)) >= 0) {
				asciiMaze[int(ascii_offset(0, 0))] = '#';
				
			}
			//The number of visited cells gets increased.
			visitedCells++;
		}
		else{
			// No available neighbours so backtrack!
			m_stack.pop();
		}
	}
}


//The start door is generated by taking in the assigned cardinal position (which will always be
//on the opposite side of the exit door).
//When a cardinal position is assessed, the method will look for an available tile, represented by
//a @ character, and use it as a starting tile.
void Maze::generateStartDoor(int position) {
	char tile = 'S';
	float it = 0;
	int posX, posY;
	switch (position)
	{
	case 0: // North
		//FIRST NORTH POINT (0,y)
		while (!(asciiMaze[int((it * 2) * (mazeWidth + 1) + (0))] == '@') && (int((it * 2) * (mazeWidth + 1) + (0)) != 460))
			it += 0.5;
		
		asciiMaze[int((it * 2) * (mazeWidth + 1) + (0))] = tile;
		posX = 0;
		posY = it * 2;
		//If the tile in front of the starting tile is a wall, I remove it. It's a pretty rare situation but
		//managing this way makes the game actually playable. This check is applied for every cardinal direction.
		if (asciiMaze[int((it * 2) * (mazeWidth + 1) + (1))] == '#') {
			asciiMaze[int((it * 2) * (mazeWidth + 1) + (1))] = '@';
		}
		break;

	case 1: // East
		//FIRST EAST POINT (x,Width)
		while (!(asciiMaze[int((mazeWidth - 1) * (mazeWidth + 1) + (it * 2))] == '@'))
			it += 0.5;
		asciiMaze[int((mazeWidth - 2) * (mazeWidth + 1) + (it * 2))] = tile;
		posX = it * 2;
		posY = mazeWidth - 2;
		if (asciiMaze[int((mazeWidth - 3) * (mazeWidth + 1) + (it * 2))] == '#') {
			asciiMaze[int((mazeWidth - 3) * (mazeWidth + 1) + (it * 2))] = '@';
		}
		break;

	case 2: // South
		//FIRST SOUTH POINT (Height, y)
		while (!(asciiMaze[int((it * 2) * (mazeWidth + 1) + (mazeHeight - 2))] == '@'))
			it += 0.5;
		asciiMaze[int((it * 2) * (mazeWidth + 1) + (mazeHeight - 2))] = tile;
		posX = mazeHeight - 2;
		posY = it * 2;
		if (asciiMaze[int((it* 2) * (mazeWidth + 1) + (mazeHeight - 3))] == '#') {
			asciiMaze[int((it * 2) * (mazeWidth + 1) + (mazeHeight - 3))] = '@';
		}
		break;

	case 3: // West 
		//FIRST WEST POINT (x,0)
		while (!(asciiMaze[int((0.5) * (mazeWidth + 1) + (it * 2))] == '@'))
			it += 0.5;
		asciiMaze[int((0) * (mazeWidth + 1) + (it * 2))] = tile;
		posX = it * 2;
		posY = 0;
		if (asciiMaze[int((1) * (mazeWidth + 1) + (it * 2))] == '#') {
			asciiMaze[int((1) * (mazeWidth + 1) + (it * 2))] = '@';
		}
		break;
	}
	//generating coordinate pair for start tile in 3D maze
	int x = 10 * (mazeHeight - posX);
	int z = 10 * (mazeWidth - posY);
	pair<int, int> start_pair_in_maze = make_pair(x, z);
    this->startTile = StartTile(start_pair_in_maze, position);
}


//The exit door is generated by taking in the assigned cardinal position (which will always be
//on the opposite side of the start door).
//When a cardinal position is assessed, the method will look for an available tile, represented by
//a @ character, and use it as an exit door.
void Maze::generateExitDoor(int position) {
	float it = 0;
	char tile = 'E';
	int posX, posY;
	switch (position)
	{
	case 0: // North
		//FIRST NORTH POINT (0,y)
		while (!(asciiMaze[int((it * 2) * (mazeWidth + 1) + (0))] == '@'))
			it += 0.5;
		asciiMaze[int((it * 2) * (mazeWidth + 1) + (0))] = tile;
		posX = 0;
		posY = it * 2;
		//If the tile in front of the exit tile is a wall, I remove it. Again, it's a pretty rare situation but
		//managing this way makes the game actually playable. This check is applied for every cardinal direction.
		if (asciiMaze[int((it * 2) * (mazeWidth + 1) + (1))] == '#') {
			asciiMaze[int((it * 2) * (mazeWidth + 1) + (1))] = '@';
		}
		
		break;

	case 1: // East
		//FIRST EAST POINT (x,Width)
		while (!(asciiMaze[int((mazeWidth - 1) * (mazeWidth + 1) + (it * 2))] == '@'))
			it += 0.5;
		asciiMaze[int((mazeWidth - 2) * (mazeWidth + 1) + (it * 2))] = tile;
		posX = it * 2;
		posY = mazeWidth - 2;
		if (asciiMaze[int((mazeWidth - 3) * (mazeWidth + 1) + (it * 2))] == '#') {
			asciiMaze[int((mazeWidth - 3) * (mazeWidth + 1) + (it * 2))] = '@';
		}
		break;

	case 2: // South
		//FIRST SOUTH POINT (Height, y)
		while (!(asciiMaze[int((it * 2) * (mazeWidth + 1) + (mazeHeight - 1))] == '@'))
			it += 0.5;
		asciiMaze[int((it * 2) * (mazeWidth + 1) + (mazeHeight - 2))] = tile;
		posX = mazeHeight - 2;
		posY = it * 2;
		if (asciiMaze[int((it * 2) * (mazeWidth + 1) + (mazeHeight - 3))] == '#') {
			asciiMaze[int((it * 2) * (mazeWidth + 1) + (mazeHeight - 3))] = '@';
		}
		break;

	case 3: // West 
		//FIRST WEST POINT (x,0)
		while (!(asciiMaze[int((0) * (mazeWidth + 1) + (it * 2))] == '@'))
			it += 0.5;
		asciiMaze[int((0) * (mazeWidth + 1) + (it * 2))] = tile;
		posX = it * 2;
		posY = 0;
		if (asciiMaze[int((1) * (mazeWidth + 1) + (it * 2))] == '#') {
			asciiMaze[int((1) * (mazeWidth + 1) + (it * 2))] = '@';
		}
		break;
	}
	
	//generating coordinate pair for the exit in 3D maze
	int x = 10 * (mazeHeight - posX);
	int z = 10 * (mazeWidth - posY);
	pair<int, int> exit_pair_in_maze = make_pair(x, z);
	this->exitTile = ExitTile(exit_pair_in_maze, 5.0, position);
}


//In this method the tile containing the key and the tiles containing the treasures will
//be defined.
//First of all, a vector of maze coordinates is built (I considered only the inner part of the
//maze) and every walkable tile coordinates are pushed back in this vector.
//Finally, this vector is shuffled 5 times and it will be passed to the keyTile and treasureTile
//methods.
void Maze::specialTiles() {
	vector<pair<int, int>> available_coords;
	//This is where the vector containing every walkable tile is built.
	//Only the inner parts of the maze are taken into account.
	for (int i = 1; i < mazeWidth - 2; i++) {
		for (int j = 1; j < mazeHeight - 2; j++) {
			if (asciiMaze[j * (mazeWidth + 1) + i] == '@')
				available_coords.push_back(make_pair(i, j));
		}
	}
	//Shuffling this vector of available coordinates
	auto rng = std::default_random_engine{};
	for(int i=0; i<5; i++)
		std::shuffle(std::begin(available_coords), std::end(available_coords), rng);

	//Key tile - Basically, a tile is randomly picked from the available_coords vector
	keyTile(available_coords, 'K');

	//Shuffling this vector of available coordinates
	rng = std::default_random_engine{};
	for (int i = 0; i < 5; i++)
		std::shuffle(std::begin(available_coords), std::end(available_coords), rng);

	//Treasure tile - Similar to the Key tile. In this case, however, "numberOfTreasures" tiles
	//will be picked randomly.
	treasureTile(available_coords, 't');
}

void Maze::keyTile(vector<pair<int, int>> &available_coords, char tile) {
	int r_it = rand() % (int(available_coords.size()));
	pair<int, int> key_pair = available_coords.at(r_it);
	available_coords.erase(available_coords.begin() + r_it);
	asciiMaze[(key_pair.second) * (mazeWidth + 1) + (key_pair.first)] = tile;
	
	//generating coordinate pair for key in 3D maze
	int x = 10 * (mazeHeight - key_pair.first);
	int z = 10 * (mazeWidth - key_pair.second);
	pair<int, int> key_pair_in_maze = make_pair(x, z);
	this->key = Key(key_pair_in_maze, 5.0);

	//If the key is surrounded by walls, which is a pretty rare situation, I randomly remove two walls
	//to make sure that the key is always reachable.
	if (asciiMaze[(key_pair.second - 1) * (mazeWidth + 1) + (key_pair.first)] == '#' && asciiMaze[(key_pair.second + 1) * (mazeWidth + 1) + (key_pair.first)] == '#'
		&& asciiMaze[(key_pair.second) * (mazeWidth + 1) + (key_pair.first - 1)] == '#' && asciiMaze[(key_pair.second) * (mazeWidth + 1) + (key_pair.first + 1)] == '#') {
		
		int randWall = rand() % 4;
		if (randWall % 2 == 0) {
			asciiMaze[(key_pair.second - 1) * (mazeWidth + 1) + (key_pair.first)] = '@';
			asciiMaze[(key_pair.second + 1) * (mazeWidth + 1) + (key_pair.first)] = '@';
		}
		else {
			asciiMaze[(key_pair.second) * (mazeWidth + 1) + (key_pair.first - 1)] = '@';
			asciiMaze[(key_pair.second) * (mazeWidth + 1) + (key_pair.first + 1)] = '@';
		}

	}
}

void Maze::treasureTile(vector<pair<int, int>> &available_coords, char tile) {
	for (int i = 0; i < numberOfTreasures; i++) {
		int treasure_it = rand() % (int(available_coords.size()));
		pair<int, int> treasure_pair = available_coords.at(treasure_it);
		available_coords.erase(available_coords.begin() + treasure_it);
		asciiMaze[(treasure_pair.second) * (mazeWidth + 1) + (treasure_pair.first)] = tile;

		//generating coordinate pair for treasure in 3D maze
		int x = 10 * (mazeHeight - treasure_pair.first);
		int z = 10 * (mazeWidth - treasure_pair.second);
		pair<int, int> treasure_pair_in_maze = make_pair(x, z);
		this->treasures[i] = Treasure(treasure_pair_in_maze, 5.0);
		//If the treasure is surrounded by walls, which is a pretty rare situation, I randomly remove two walls
		//to make sure that the gem is always reachable.
		if (asciiMaze[(treasure_pair.second - 1) * (mazeWidth + 1) + (treasure_pair.first)] == '#' && asciiMaze[(treasure_pair.second + 1) * (mazeWidth + 1) + (treasure_pair.first)] == '#'
			&& asciiMaze[(treasure_pair.second) * (mazeWidth + 1) + (treasure_pair.first - 1)] == '#' && asciiMaze[(treasure_pair.second) * (mazeWidth + 1) + (treasure_pair.first + 1)] == '#') {

			int randWall = rand() % 4;
			if (randWall % 2 == 0) {
				asciiMaze[(treasure_pair.second - 1) * (mazeWidth + 1) + (treasure_pair.first)] = '@';
				asciiMaze[(treasure_pair.second + 1) * (mazeWidth + 1) + (treasure_pair.first)] = '@';
			}
			else {
				asciiMaze[(treasure_pair.second) * (mazeWidth + 1) + (treasure_pair.first - 1)] = '@';
				asciiMaze[(treasure_pair.second) * (mazeWidth + 1) + (treasure_pair.first + 1)] = '@';
			}

		}
	}
}

//The planes include the floor, the ceiling and the four external walls.
void Maze::setupPlanes() {
	//I'm setting the correct length for the 3D maze
	int len = ((this->mazeWidth) * 10) + 5;
	this->floor = PlaneDrawer(len, -5, -len);
	this->ceiling = PlaneDrawer(len, -5, -len);
	//this for is used for the external walls
	for (int i = 0; i < 4; i++) {
		this->walls.push_back(ExternalWall(len, 10, 1, i));
	}
}

void Maze::print() {
	//Printing instructions
	cout << "Welcome to MazeGL! Here's the map legend:" << endl;
	cout << "@ -> Walkable tile" << endl;
	cout << "# -> Wall tile" << endl;
	cout << "S -> Start tile" << endl;
	cout << "E -> Exit tile" << endl;
	cout << "K -> Key tile" << endl;
	cout << "t -> Treasure tile" << endl;
	cout << "Map:" << endl;
	//Printing the maze on the console
	for (int i = 0; i < mazeHeight - 1; i++) {
		for (int j = 0; j < mazeWidth - 1; j++) {
			std::cout << asciiMaze[j * (mazeWidth + 1) + i] << ' ';
		}
		std::cout << endl;
	}
	cout << "Good luck!" << endl;
}
