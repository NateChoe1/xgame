#define UP_PRESSED 0
#define RIGHT_PRESSED 1
#define LEFT_PRESSED 2
#define TOTAL_CONTROLS 3

#define WINDOW_X 300
#define WINDOW_Y 300
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 400
#define START_X 240
#define START_Y 390
#define JUMP_POWER 15
#define PLAYER_SIZE 10

void updateKeysPressed(Display *dpy, char keysPressed[3]);
void updatePosition(Display *dpy, char keysPressed[3], int width, int height,
			int *x, int *y, int *ySpeed);
void initVars(Display *dpy, Window root, Window win);
