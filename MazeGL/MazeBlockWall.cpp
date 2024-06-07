#include "MazeBlockWall.h"


MazeBlockWall::MazeBlockWall(float x, float y, float z) {
	this->length = 10;
	this->centerX = x;
	this->centerY = y;
	this->centerZ = z;
}

void MazeBlockWall::drawBlock()
{
	glPushMatrix();
		glTranslatef(this->centerX, this->centerY, this->centerZ);
		drawSolidCube();
	glPopMatrix();
}

//Collision Check.
//The player is associated with a Bounding Sphere and each geometry has its own Bounding Box. The collision detection approach
//is a simple collision detection method between a sphere and a cube.
bool MazeBlockWall::checkCollisionWithPlayer(tuple<GLfloat, GLfloat> playerCoordinates, float playerBB) {
	float x_playerCoordinate = get<0>(playerCoordinates);
	float z_playerCoordinate = get<1>(playerCoordinates);

	//This has to be an input parameter (along with the other dimensions)
	float l = (this->length) / 2;

	//Checking the distance between the player and the wall, coordinate by coordinate.
	float sphereXDistance = abs(x_playerCoordinate - this->centerX);
	float sphereYDistance = abs(0 - this->centerY);
	float sphereZDistance = abs(z_playerCoordinate - this->centerZ);
	//Checking if the sphere is inside the cube or not
	//Outside
	if (sphereXDistance >= (l + playerBB)) { return false; }
	if (sphereYDistance >= (l + playerBB)) { return false; }
	if (sphereZDistance >= (l + playerBB)) { return false; }
	//Inside
	if (sphereXDistance < (l)) { return true; }
	if (sphereYDistance < (l)) { return true; }
	if (sphereZDistance < (l)) { return true; }

	//Checking the distance between the sphere and each corner of the cube.
	float cornerDistance_sq = ((sphereXDistance - l) * (sphereXDistance - l)) +
		((sphereYDistance - l) * (sphereYDistance - l) +
			((sphereYDistance - l) * (sphereYDistance - l)));
	//Returning if I'm colliding or not with a corner of the cube.
	return (cornerDistance_sq < (playerBB* playerBB));
}



//Implementing a custom version of glutSolidCube allowing for easy texture mapping
void MazeBlockWall::drawBox(GLenum type)
{
	static GLfloat n[6][3] = // Normals
	{
	  {-1.0, 0.0, 0.0},//back face
	  {0.0, 1.0, 0.0}, //up face
	  {1.0, 0.0, 0.0},//front face
	  {0.0, -1.0, 0.0},//down face
	  {0.0, 0.0, 1.0},//right face
	  {0.0, 0.0, -1.0} //left face
	};
	static GLint faces[6][4] =
	{
	  {0, 1, 2, 3}, //back face
	  {3, 2, 6, 7}, //up face
	  {7, 6, 5, 4}, //front face
	  {4, 5, 1, 0}, //down face
	  {5, 6, 2, 1}, //right face
	  {7, 4, 0, 3}  //left face
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -(this->length) / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = (this->length) / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -(this->length) / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = (this->length) / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -(this->length) / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = (this->length) / 2;

	
	//Applying texture, making sure that each face displays the texture itself properly
	//Back Face
	glBegin(type);
		glNormal3fv(&n[0][0]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(&v[faces[0][0]][0]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(&v[faces[0][1]][0]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(&v[faces[0][2]][0]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(&v[faces[0][3]][0]);
	glEnd();

	//Front Face
	glBegin(type);
		glNormal3fv(&n[2][0]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(&v[faces[2][0]][0]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(&v[faces[2][1]][0]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(&v[faces[2][2]][0]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(&v[faces[2][3]][0]);
	glEnd();

	//Right Face
	glBegin(type);
		glNormal3fv(&n[4][0]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(&v[faces[4][0]][0]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(&v[faces[4][1]][0]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(&v[faces[4][2]][0]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(&v[faces[4][3]][0]);
	glEnd();


	//Left Face
	glBegin(type);
		glNormal3fv(&n[5][0]);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(&v[faces[5][0]][0]);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(&v[faces[5][1]][0]);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(&v[faces[5][2]][0]);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(&v[faces[5][3]][0]);
	glEnd();

}

void MazeBlockWall::drawSolidCube()
{
	drawBox(GL_QUADS);
}