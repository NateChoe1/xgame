#define UP_PRESSED 0
#define RIGHT_PRESSED 1
#define LEFT_PRESSED 2
#define TOTAL_CONTROLS 3

void updateKeysPressed(Display *dpy, char keysPressed[3]);
void updatePosition(Display *dpy, char keysPressed[3], int width, int height,
			int *x, int *y, int *ySpeed);
