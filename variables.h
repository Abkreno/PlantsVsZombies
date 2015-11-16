#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600
#define MAX_NUM_OF_VIEWS  3
float eps = 1;
float zoom = -20.0f;
float rotx = 30;
float roty = 90.001f;
float rotz = 0;
float dx = 0.05;
float dy = 0.05;
float dzoom = 0.05;
int xDir = 1;
int yDir = 1;
int zoomDir = 1;
float views[3][MAX_NUM_OF_VIEWS];
int lastx = 0;
int lasty = 0;
int currView = 0;
unsigned char Buttons[3] = { 0 };

bool view = false;