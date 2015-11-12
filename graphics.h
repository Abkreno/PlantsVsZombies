bool paused = false;
struct Defender {
	float x, y, z, boltZ ,dt;
	bool ballVanished;
	bool enemyExists;
	Defender::Defender() {}
	Defender::Defender(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->boltZ = 1000;
		this->dt = 1000;
		this->ballVanished = false;
		this->enemyExists = false;
	}

	void updateBoltZ() {
		boltZ += boltZ<2000 ?0.029:0;
		dt += dt<2000 ?0.5:0.0;
		if (dt >= 1000.0f && enemyExists) {
			dt = 0.0f;
			boltZ = 0.0f;
			ballVanished = false;
		}
	}
	void stopAttack() {
		enemyExists = false;
	}
	void attack() {
		enemyExists = true;
	}
	void drawBolt() {
		glPushMatrix();
		glColor3f(0.13f, 0.13f, 0.13f);
		glTranslatef(0.0f, 1.0f, boltZ);
		glScalef(0.2, 0.2, 0.6);
		if(!paused)
			updateBoltZ();
		if (!ballVanished)
			gluSphere(gluNewQuadric(), 0.3f, 30, 30);
		glPopMatrix();
	}
	void drawGun() {
		glTranslatef(0, 1, 0);
		glScalef(0.1, 0.1, 0.4);
		gluCylinder(gluNewQuadric(), 1, 1, 2, 30, 30);
	}
	void drawBody() {
		glTranslatef(0, 1, 0);
		glScalef(0.2, 1, 0.2);
		glPushMatrix();
		glScalef(1, 0.2, 1);
		gluSphere(gluNewQuadric(), 1.0f, 30, 30);
		glPopMatrix();
		glRotatef(90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 1, 1, 2, 30, 30);
	}
	void draw() {
		glPushMatrix();
		glPushMatrix();
		drawBolt();
		glColor3f(0.7, 0.1, 0.1);
		drawGun();
		glPopMatrix();
		glPushMatrix();
		drawBody();
		glPopMatrix();
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
	void updateRotateAng() {
		if (dt >= 1.0f) {
			dt = 0.0f;
			rotAng += 10.0f;
		}
		else {
			dt += 0.01f;
		}
	}
	void draw() {
		glPushMatrix();
		glRotatef(rotAng, 0.0f, 1.0f, 0.0f);
		if (!paused)
			updateRotateAng();
		glPushMatrix();
		glColor3f(0.1f, 0.01f, 0.5f);
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 1.3f, 11, 11);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(0.0f, 1.5f, 0.0f);
		glScalef(0.2f, 0.2f, 0.2f);
		glColor3f(0.1f, 0.01f, 0.5f);
		glutSolidIcosahedron();
		glPopMatrix();
		
		glPopMatrix();
	}
};
struct Tile {
	float x, y, z, r, b, g;
	bool highlighted, occupied,hasEnemy;
	char character;
	Defender defender;
	ResourceGatherer resourceG;
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
		this->hasEnemy = false;
		this->character = '0';
		this->defender = Defender(x,y,z);
		this->resourceG = ResourceGatherer(x, y, z);
	}
	void drawCharacter() {
		if (character == 'd' || character == 'D') {
			defender.draw();
		}
		else if (character == 'r' || character == 'R') {
			resourceG.draw();
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
