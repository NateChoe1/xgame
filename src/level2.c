#include <unistd.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#include "help.h"
#include "levels.h"
#include "windowstop.h"

void level2(Display *dpy, Window root, Window win, Window player, int screen) {
	Window unusedWindow;
	int unusedInt;
	unsigned long unusedLong;

	char keysPressed[TOTAL_CONTROLS] = {0};
	int width, height;
	int windowX, windowY;
	int x = WINDOW_X + START_X, y = WINDOW_Y + START_Y, ySpeed = 0;
	int oldX = x, oldY = y;

	moveResizeWindow(dpy, root, win,
			WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT);

	for (;;) {
		XRaiseWindow(dpy, win);
		XRaiseWindow(dpy, player);
		if (y > HeightOfScreen(ScreenOfDisplay(dpy, screen))) {
			x = WINDOW_X + START_X;
			y = WINDOW_Y + START_Y;
			ySpeed = 0;
		}
		XGetGeometry(dpy, win, &unusedWindow, &unusedInt, &unusedInt,
				&width, &height,
				&unusedInt, &unusedInt);
		XTranslateCoordinates(dpy, win, root, 0, 0,
				&windowX, &windowY, &unusedWindow);
		if (windowX != WINDOW_X || windowY != WINDOW_Y ||
				width != WINDOW_WIDTH || height != WINDOW_HEIGHT)
			moveResizeWindow(dpy, root, win,
					WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT);
		//this will probably break on fvwm, but this entire game will probably
		//break on every window manager.
		updateKeysPressed(dpy, keysPressed);
		updatePosition(dpy, keysPressed, width, height, &x, &y, &ySpeed);

		int borderCount;
		Border *borders = createBorders(dpy, root, win, &borderCount);

		int stoppedBorder = stoppingBorder(borders, borderCount,
				x, y, oldY, win);
		if (stoppedBorder >= 0 && ySpeed >= 0) {
			y = borders[stoppedBorder].y - 10;
			ySpeed = 0;
		}
		if (ySpeed == 0 && onBound(borders, borderCount, x, y)) {
			if (keysPressed[UP_PRESSED]) {
				ySpeed = -JUMP_POWER;
				standingOnBorder = 0;
			}
		}
		else
			ySpeed++;
	
		XMoveWindow(dpy, player, x, y);
		oldX = x;
		oldY = y;

		if (y < WINDOW_Y && BETWEEN(x, WINDOW_X, WINDOW_X + WINDOW_WIDTH))
			return;

		usleep(16666);
	}
}
