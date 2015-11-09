struct Tile {
	float x, y, z, r, b, g;
	bool highlighted, occupied;
	Tile::Tile() {}
	Tile::Tile(float x, float y, float z, float r, float g, float b) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
		this->g = g;
		this->b = b;
		highlighted = false;
		occupied = false;
	}

	void draw() {
		glPushMatrix();
		glTranslatef(x, y, z);
		if (highlighted) {
			glColor3f(0.3, 0.3, 0.3);
		}
		else {
			glColor3f(r, g, b);
		}
		glScalef(0.9, 0.05f, 0.9);
		glutSolidCube(1);
		glPopMatrix();
	}
};
