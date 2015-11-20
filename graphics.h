#include <variables.h>

const int MAXMONSTERS = 10;
const float HouseZ = -5.0f;
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
		this->dz = 0;
		this->dt = 0;
		this->ballVanished = false;
		this->enemyExists = false;
	}

	void updateBoltZ() {
		dz += dz<=200 ?0.2:0;
		dt += dt<=50?1:0;
		if (dt > 50 && enemyExists) {
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
		if (!paused&&!game_over)
			updateBoltZ();
		glPushMatrix();
		glColor3f(0.13f, 0.13f, 0.13f);
		glTranslatef(0.0f, 1.0f, dz);
		glScalef(0.2, 0.2, 0.6);
		if (!ballVanished&&!game_over)
			gluSphere(gluNewQuadric(), 0.3f, 30, 30);
		glPopMatrix();
	}
	void drawGun() {
		glTranslatef(0, 1, 0);
		glScalef(0.1, 0.1, 0.2);
		gluCylinder(gluNewQuadric(), 1, 1, 2, 30, 30);
	}
	void drawBody() {
		glTranslatef(0, 1, 0);
		glScalef(0.2, 1, 0.2);
		glPushMatrix();
		glScalef(1, 0.2, 1);
		gluSphere(gluNewQuadric(), 1.0f, 30, 30);
		glPopMatrix();
		glPushMatrix();
		glScalef(0.9, 1, 0.9);
		glRotatef(90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 0.8, 1.5, 2, 30, 30);
		glPopMatrix();
	}
	void draw(float HP) {
		glPushMatrix();
		glPushMatrix();
		drawBolt();
		glColor3f(0.7-HP, 0.1-HP, 0.1-HP);
		drawGun();
		glPopMatrix();
		glPushMatrix();
		drawBody();
		glPopMatrix();
		glPopMatrix();
	}
};
struct Shield {
	float x, y, z, rotAng, dt;
	Shield::Shield() {}
	Shield::Shield(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->dt = 0.0f;
	}
	void draw(float HP) {
		glPushMatrix();
		glTranslatef(0.0f, 0.9f, 0.0f);
		glScalef(1.0f, 1.0f, 0.2f);
		glColor3f(0.2 + HP, 0.2 + HP, 0.2 + HP);
		glutSolidIcosahedron();
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
		rotAng += 10.0f;
	}
	void drawZibber() {
		glPushMatrix();
		glTranslatef(0.0f, 0.1f, 0.0f);
		glScalef(0.5, 0.5, 0.5);
		float height = 0.0f;
		for (int i = 0; i < 100; i++) {
			glPushMatrix();
			glTranslatef(0.0f, height, 0.0f);
			glRotatef(-7 * i, 0, 1, 0);
			glTranslatef(0.1, 0, 0);
			glutSolidSphere(0.08, 30, 30);
			height += 0.019;
			glPopMatrix();
		}
		glPopMatrix();
	}
	void updateMoney() {
		dt += 0.01;
		if (dt >= 1) {
			dt = 0;
			money += 20;
		}
	}
	void draw(float HP) {
		glPushMatrix();
		glTranslatef(0, -0.05, 0);
		glRotatef(rotAng, 0.0f, 1.0f, 0.0f);
		if (!paused) {
			updateRotateAng();
			updateMoney();
		}
		glColor3f(0.7-HP, 0.1 - HP, 0.1- HP);
		
		drawZibber();

		glPushMatrix();
		glTranslatef(0.0f, 1.2f, 0.0f);
		glScalef(0.2f, 0.2f, 0.2f);
		glColor3f(0.7 + HP, 0.1 + HP, 0.1 + HP);
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
	Shield shield;
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
		this->shield = Shield(x, y, z);
	}
	void addCharacter(char c) {
		if (occupied||costs[c - 'a']>money)
			return;
		money -= costs[c - 'a'];
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
		if ((character == 's'&&characterHP > 2)
			||(character!='s'&&characterHP > 0.4))
			destroyCharacter();
	}
	float minimum(float x, float y) {
		if (x < y)return x;
		return y;
	}
	void drawCharacter() {
		if (character == 'd' || character == 'D') {
			defender.draw(characterHP);
		}
		else if (character == 'r' || character == 'R') {
			resourceG.draw(characterHP);
		}
		else if (character == 's' || character == 'S') {
			shield.draw(characterHP*0.3);
		}
	}
	void drawDestroyed() {
		glPushMatrix();
		glTranslatef(x, y, z);
		glColor3f(r-0.4, g-0.2, b-0.5);
		glScalef(0.9, 0.05f, 0.9);
		glutSolidCube(1);
		glPopMatrix();
	}
	void draw() {
		glPushMatrix();
		glTranslatef(x, y, z);
		if (occupied) {
			drawCharacter();
		}
		if (highlighted) {
			glColor4f(r-0.3, g-0.3, b-0.3,0.2);
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
	float x, y, z,r,g,HP,b,dz,dt,rotAng;
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
		this->rotAng = 0;
		this->isDead = true;
		this->stop = false;
	}
	void start() {
		isDead = false;
		dz = 0;
		dt = 10;
		HP = 0;
	}
	void update() {
		dz-= 0.0045;
	}
	void decreaseHP() {
		HP += 0.1;
		if (HP >= 0.3)
			isDead = true;
	}
	void drawWheel(float x, float z,float diskZ) {
		glPushMatrix();	
		
		glRotatef(90, 0, 1, 0);
		glScalef(0.15, 0.15, 0.15);
		glTranslatef(x, -3.0f, z);
		gluCylinder(gluNewQuadric(), 1, 1, 1, 30, 30);
		glPushMatrix();
		glTranslatef(0, 0, diskZ);
		gluDisk(gluNewQuadric(), 0, 1, 30, 30);
		glPopMatrix();
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
	void drawTop() {
		glPushMatrix();
		glTranslatef(0, 0.5, 0);
		glScalef(0.2, 0.3, 0.2);
		glutSolidDodecahedron();
		glPopMatrix();
	}
	void draw() {
		glPushMatrix();
		if(!paused&&!stop)
			update();
		glColor3f(r-HP, g-HP, b-HP);
		glTranslatef(x, y+0.6, z+dz);
		glScalef(0.9, 1, 1);
		glutSolidCube(1);
		drawTop();
		drawCone(1.6f);
		drawCone(0);
		drawCone(-1.6f);
		drawWheel(1.8, 3.2,1);
		drawWheel(-2.0, 3.2,1);
		drawWheel(-2.0, -4.2,0);
		drawWheel(1.8, -4.2,0);
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
				numOfMonsters++;
				totalMonsters++;
				return;
			}
		}
	}
	void drawMonsters() {
		if (game_over)
			return;
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
void incrementScore();
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
							if (monsterFactories[i].monsters[k].isDead) {
								monsterFactories[i].numOfMonsters--;
								totalMonsters--;
								incrementScore();
								money += 10;
							}
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
				laneDestroyed[i] = true;
				for (int k = 0;k < MAXMONSTERS; k++) {
					if (monsterFactories[i].monsters[k].isDead)
						continue;
					monsterFactories[i].monsters[k].isDead = true;
					monsterFactories[i].numOfMonsters--;
					totalMonsters--;
				}
				destroyed_lanes++; 
				if (selected_row == i) {
					tiles[selected_row][selected_col].highlighted = false;
					currState = SELECT_ROW;
				}
				if (destroyed_lanes == 3)
					game_over = true;
				break;
			}
			for (int k = 0; k < gridCols; k++) {
				if (tiles[i][k].occupied&&intersects(tiles[i][k].z + 0.5,
					monsterFactories[i].monsters[j].z + monsterFactories[i].monsters[j].dz)) {
					monsterFactories[i].monsters[j].stop = true;
					monsterFactories[i].monsters[j].dt++;
					if (monsterFactories[i].monsters[j].dt >= 10) {
						tiles[i][k].decreaseHP();
						monsterFactories[i].monsters[j].dt = 0;
					}
					break;
				}
			}
			
		}
	}
}

