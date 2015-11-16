const int MAXMONSTERS = 10;
const float HouseZ = -5.0f;
const int gridRows = 5, gridCols = 9;
bool paused = false;
bool laneDestroyed[gridRows];
struct Defender {
	float x, y, z, dz;
	int dt;
	bool ballVanished;
	bool enemyExists;
	Defender::Defender() {}
	Defender::Defender(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->dz = 300;
		this->dt = 0;
		this->ballVanished = false;
		this->enemyExists = false;
	}

	void updateBoltZ() {
		dz += dz<=200 ?0.2:0;
		dt += dt<=500?10:0;
		if (dt > 500 && enemyExists) {
			dt = 0;
			dz = 0.0f;
			ballVanished = false;
		}
	}
	void stopAttack() {
		enemyExists = false;
	}
	void startAttack() {
		enemyExists = true;
	}
	void drawBolt() {
		if (!paused)
			updateBoltZ();
		glPushMatrix();
		glColor3f(0.13f, 0.13f, 0.13f);
		glTranslatef(0.0f, 1.0f, dz);
		glScalef(0.2, 0.2, 0.6);
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
	void draw(float HP) {
		glPushMatrix();
		glPushMatrix();
		drawBolt();
		glColor3f(0.7+HP, 0.1+HP, 0.1+HP);
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
		if (dt >= 2.0f) {
			dt = 0.0f;
			rotAng += 10.0f;
		}
		else {
			dt += 1.7f;
		}
	}
	void draw(float alpha) {
		glPushMatrix();
		glRotatef(rotAng, 0.0f, 1.0f, 0.0f);
		if (!paused)
			updateRotateAng();
		glPushMatrix();
		glColor4f(0.7, 0.1, 0.1,alpha);
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		gluCylinder(gluNewQuadric(), 0.1f, 0.1f, 1.3f, 11, 11);
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(0.0f, 1.5f, 0.0f);
		glScalef(0.2f, 0.2f, 0.2f);
		glColor4f(0.7, 0.1, 0.1, alpha);
		glutSolidIcosahedron();
		glPopMatrix();
		
		glPopMatrix();
	}
};

struct Tile {
	float x, y, z, r, b, g,characterHP;
	bool highlighted, occupied;
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
		this->character = '0';
		this->characterHP = 0;
		this->defender = Defender(x,y,z);
		this->resourceG = ResourceGatherer(x, y, z);
	}
	void addCharacter(char c) {
		character = c;
		characterHP = 0;
		occupied = true;
	}
	void destroyCharacter() {
		character = '0';
		occupied = false;
		characterHP = 1;
	}
	void decreaseHP() {
		characterHP += 0.1;
		if (characterHP > 0.3)
			destroyCharacter();
	}
	void drawCharacter() {
		if (character == 'd' || character == 'D') {
			defender.draw(characterHP);
		}
		else if (character == 'r' || character == 'R') {
			resourceG.draw(characterHP);
		}
	}
	void draw() {
		glPushMatrix();
		glTranslatef(x, y, z);
		if (occupied) {
			drawCharacter();
		}
		if (highlighted) {
			glColor4f(r, g, b,0.5);
		}
		else {
			glColor4f(r, g, b,1.0);
		}

		glScalef(0.9, 0.05f, 0.9);
		glutSolidCube(1);
		glPopMatrix();
	}
}tiles[gridRows][gridCols];
struct Monster {
	float x, y, z,r,g,HP,b,dz,dt;
	bool isDead,stop;
	Monster::Monster() {}
	Monster::Monster(float x, float y, float z,float r,float g,float b) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
		this->g = g;
		this->b = b;
		this->dt = 0;
		this->dz = 0;
		this->HP = 1;
		this->isDead = true;
		this->stop = false;
	}
	void start() {
		isDead = false;
		dz = 0;
		dt = 0;
		HP = 0;
	}
	void update() {
		dz-= 0.0025;
	}
	void decreaseHP() {
		HP += 0.1;
		if (HP >= 0.3)
			isDead = true;
	}
	void drawWheel(float x, float z) {
		glPushMatrix();	
		
		glRotatef(90, 0, 1, 0);
		glScalef(0.15, 0.15, 0.15);
		glTranslatef(x, -3.0f, z);
		gluCylinder(gluNewQuadric(), 1, 1, 1, 30, 30);
		glPopMatrix();
	}

	void drawCone(float x) {
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glScalef(0.15, 0.15, 0.15);
		glTranslatef(x, 0, 3.3f);
		glutSolidCone(0.8, 1.8, 30, 30);

		glPopMatrix();
	}
	void draw() {
		glPushMatrix();
		if(!paused&&!stop)
			update();
		glColor3f(r+HP, g+HP, b+HP);
		glTranslatef(x, y+0.6, z+dz);
		glScalef(1, 1, 1);
		glutSolidCube(1);
		drawWheel(1.8, 3.2);
		drawWheel(-2.0, 3.2);
		drawWheel(-2.0, -4.2);
		drawWheel(1.8, -4.2);
		drawCone(1.6f);
		drawCone(0);
		drawCone(-1.6f);
		glPopMatrix();
	}
};
struct MonsterFactory {
	bool hasMonster;
	float x, y, z;
	int numOfMonsters;
	Monster monsters[MAXMONSTERS];
	MonsterFactory::MonsterFactory(){}
	MonsterFactory::MonsterFactory(float x,float y,float z){
		this->x = x;
		this->y = y;
		this->z = z;
		this->hasMonster = false;
		this->numOfMonsters = 0;
		for (int i = 0; i < MAXMONSTERS; i++) {
			monsters[i] = Monster(x, y, z, 0.7, 0.1, 0.1);
		}
	}
	void addMonster() {
		for (int i = 0; i < MAXMONSTERS; i++) {
			if (monsters[i].isDead) {
				monsters[i].start();
				hasMonster = true;
				return;
			}
		}
	}
	void drawMonsters() {
		glPushMatrix();
		for (int i = 0; i < MAXMONSTERS; i++)
			if(!monsters[i].isDead)
				monsters[i].draw();
		glPopMatrix();
	}
}monsterFactories[gridRows];

bool intersects(float z1, float z2) {
	if (fabs(z1 - z2) <= 0.2)
		return true;
	return false;
}
void detectBoltIntersections() {
	for (int i = 0; i < gridRows; i++) {
		if (laneDestroyed[i])
			continue;
		for (int j = 0; j < gridCols; j++) {
			if (tiles[i][j].character == 'd') {
				if (tiles[i][j].defender.enemyExists && !tiles[i][j].defender.ballVanished) {
					for (int k = 0; k < MAXMONSTERS; k++)
					{
						if (monsterFactories[i].monsters[k].isDead)
							continue;
						if (intersects(monsterFactories[i].monsters[k].z + monsterFactories[i].monsters[k].dz,
							tiles[i][j].defender.z + tiles[i][j].defender.dz)) {
							tiles[i][j].defender.ballVanished = true;
							monsterFactories[i].monsters[k].decreaseHP();
							break;
						}
					}
				}
			}
		}
		monsterFactories[i].hasMonster = false;
		for (int k = 0; k < MAXMONSTERS; k++)
		{
			monsterFactories[i].hasMonster |= !monsterFactories[i].monsters[k].isDead;
		}
	}
}
void detectMonstersIntersections() {
	for (int i = 0; i < gridRows; i++) {
		if (laneDestroyed[i])
			continue;
		for (int j = 0; j < MAXMONSTERS; j++) {
			if (monsterFactories[i].monsters[j].isDead)
				continue;
			monsterFactories[i].monsters[j].stop = false;
			if (intersects(monsterFactories[i].monsters[j].z + monsterFactories[i].monsters[j].dz, HouseZ)) {
				monsterFactories[i].monsters[j].isDead = true;
				laneDestroyed[i] = true;
				break;
			}
			for (int k = 0; k < gridCols; k++) {
				if (tiles[i][k].occupied&&intersects(tiles[i][k].z + 0.5,
					monsterFactories[i].monsters[j].z + monsterFactories[i].monsters[j].dz)) {
					monsterFactories[i].monsters[j].stop = true;
					monsterFactories[i].monsters[j].dt++;
					if (monsterFactories[i].monsters[j].dt >= 40) {
						tiles[i][k].decreaseHP();
						monsterFactories[i].monsters[j].dt = 0;
					}
					break;
				}
			}
			
		}
	}
}

