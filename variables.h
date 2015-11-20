#define ESC_KEY 27
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600
#define MAX_NUM_OF_VIEWS  18
#define SELECT_ROW 0
#define SELECT_COL 1
#define SELECT_CHR 2
char temp_buffer[300];
float eps = 1;
float zoom = -20.0f;
float rotx = 30;
float roty = 80.001f;
float rotz = 0;
float dx = 0.7;
float dy = 0.7;
float dzoom = 0.05;
float views[3][MAX_NUM_OF_VIEWS];
const int defender_cost = 200;
const int resource_gatherer_cost = 50;
const int warrior_cost = 250;
const int sheild_cost = 150;
const int max_score = 50;
const int gridRows = 5, gridCols = 9;
int score = 0;
int costs[26];
void initCosts(){
	costs['d' - 'a'] = defender_cost;
	costs['r' - 'a'] = resource_gatherer_cost;
	costs['w' - 'a'] = warrior_cost;
	costs['s' - 'a'] = sheild_cost;
}
int money = 500;
int xDir = 1;
int yDir = 1;
int zoomDir = 1;
int lastx = 0;
int lasty = 0;
int currView = 0;
int currState = 0;
int selected_row = 0;
int selected_col = 0;
int destroyed_lanes = 0;
int totalMonsters = 0;
int mod = 100;
unsigned char Buttons[3] = { 0 };
bool view = false;
bool show_prices = true;
bool game_over = false;
int level = 1;
int time = 0;
int tipsTime = 0;