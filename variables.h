#define ESC_KEY 27
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600
#define MAX_NUM_OF_VIEWS  18
#define SELECT_ROW 0
#define SELECT_COL 1
#define SELECT_CHR 2
float eps = 1;
float zoom = -20.0f;
float rotx = 30;
float roty = 90.001f;
float rotz = 0;
float dx = 0.5;
float dy = 0.5;
float dzoom = 0.05;
float views[3][MAX_NUM_OF_VIEWS];
int xDir = 1;
int yDir = 1;
int zoomDir = 1;
int lastx = 0;
int lasty = 0;
int currView = 0;
int currState = 0;
int selected_row = 0;
int selected_col = 0;
unsigned char Buttons[3] = { 0 };
bool view = false;