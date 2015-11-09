struct Defender {
	float x, y, z, boltZ ,dt;
	Defender::Defender() {}
	Defender::Defender(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->boltZ = 0;
		this->dt = 0.0f;
	}
	void draw() {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, 1.0f, boltZ);
		boltZ -= 0.009f;
		dt += 0.5f;
		if (dt >= 1000.0f) {
			dt = 0.0f;
			boltZ = 0.0f;
		}
		gluSphere(gluNewQuadric(), 0.1f, 30, 30);
		glPopMatrix();
	}
};
struct ResourceGatherer {
	float x, y, z,rotAng,dt;
	ResourceGatherer::ResourceGatherer() {}
	ResourceGatherer::ResourceGatherer(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->rotAng = 0.0f;
		this->dt = 0.0f;
	}
	void draw() {
		glPushMatrix();
		glRotatef(rotAng, 0.0f, 1.0f, 0.0f);
		
		if (dt >= 1.0f) {
			dt = 0.0f;
			rotAng += 10.0f;
		}
		else {
			dt += 0.01f;
		}
		glPushMatrix();
		glColor3f(0.2f, 0.2f, 0.2f);
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 1.3f, 11, 11);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(0.0f, 1.5f, 0.0f);
		glScalef(0.2f, 0.2f, 0.2f);
		glColor3f(0.2f, 0.2f, 0.2f);
		glutSolidDodecahedron();
		glPopMatrix();
		
		glPopMatrix();
	}
};
struct Tile {
	float x, y, z, r, b, g;
	bool highlighted, occupied;
	char character;
	Defender defender;
	ResourceGatherer resource_gatherer;
	Tile::Tile() {}
	Tile::Tile(float x, float y, float z, float r, float g, float b) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
		this->g = g;
		this->b = b;
		this->highlighted = false;
		this->occupied = false;
		this->character = '0';
		this->defender = Defender(x,y,z);
		this->resource_gatherer = ResourceGatherer(x, y, z);
	}
	void drawCharacter() {
		if (character == 'd' || character == 'D') {
			defender.draw();
		}
		else if (character == 'r' || character == 'R') {
			resource_gatherer.draw();
		}
	}
	void draw() {
		glPushMatrix();
		glTranslatef(x, y, z);
		if (occupied) {
			drawCharacter();
		}
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
