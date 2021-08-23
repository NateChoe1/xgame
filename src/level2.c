#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "help.h"
#include "levels.h"

#define BETWEEN(value, min, max) ((min <= value) && (value < max))

typedef struct {
	int left;
	int right;
	int y;
	Window win;
} Border;
//just a wall that the player can't fall down through.

Window standingWindow;
char standingOnBorder;

char borderWorks(Border border, int x, int y, int oldY) {
	if (BETWEEN(x, border.left - PLAYER_SIZE, border.right))
		if (oldY <= border.y - PLAYER_SIZE &&
		    y >= border.y - PLAYER_SIZE)
			return 1;
	return 0;
}

int stoppingBorder(Border *borders, int nborders,
		int x, int y, int oldY, Window win) {
	if (standingOnBorder) {
		for (int i = 0; i < nborders; i++)
			if (borders[i].win == standingWindow && borders[i].win != win)
				if (BETWEEN(x, borders[i].left, borders[i].right)) 
					return i;
		//for each border, if the current border matches windows, and the player
		//hasn't fallen off or anything, then return the current border being
		//stood on.
		standingOnBorder = 0;
	}
	for (int i = 0; i < nborders; i++)
		if (borderWorks(borders[i], x, y, oldY)) {
			standingOnBorder = 1;
			standingWindow = borders[i].win;
			return i;
		}
	return -1;
}
char onBound(Border *borders, int nborders, int x, int y) {
	for (int i = 0; i < nborders; i++)
		if (borderWorks(borders[i], x, y, y))
			return 1;
	return 0;
}

void level2(Display *dpy, Window root, Window win, Window player, int screen) {
	Window unusedWindow;
	int unusedInt;
	unsigned long unusedLong;

	Window *topLevels;
	unsigned int topCount;
	char keysPressed[TOTAL_CONTROLS] = {0};
	int width, height;
	int windowX, windowY;
	int x = WINDOW_X + START_X, y = WINDOW_Y + START_Y, ySpeed = 0;
	int oldX = x, oldY = y;

	moveResizeWindow(dpy, root, win,
			WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT);

	for (;;) {
		if (standingOnBorder) {
			XTextProperty prop;
			XGetTextProperty(dpy, standingWindow, &prop, atoms[_NET_WM_NAME]);
			puts(prop.value);
		}

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
		XQueryTree(dpy, root, &unusedWindow, &unusedWindow,
				&topLevels, &topCount);
		//this will probably break on fvwm, but this entire game will probably
		//break on every window manager.
		updateKeysPressed(dpy, keysPressed);
		updatePosition(dpy, keysPressed, width, height, &x, &y, &ySpeed);

		Border *borders = malloc(sizeof(Border) * (topCount + 1));
		int borderCount = 0;
		borders[borderCount++] = (Border) {
			.left = WINDOW_X,
			.right = WINDOW_X + width,
			.y = WINDOW_Y + height,
			.win = win,
		};
		for (int i = 0; i < topCount; i++) {
			int currentWidth, currentHeight, currentX, currentY, borderWidth;
			XGetGeometry(
					dpy, topLevels[i], &unusedWindow,
					&currentX, &currentY,
					&currentWidth, &currentHeight,
					&borderWidth,
					&unusedInt
			);
			borders[borderCount++] = (Border) {
				.left = currentX - borderWidth,
				.right = currentX + currentWidth + borderWidth,
				.y = currentY,
				.win = topLevels[i],
			};
		}
		XFree(topLevels);

		int stoppedBorder = stoppingBorder(borders, topCount + 1,
				x, y, oldY, win);
		if (stoppedBorder >= 0 && ySpeed > 0) {
			y = borders[stoppedBorder].y - 10;
			ySpeed = 0;
		}
		if (ySpeed == 0 && onBound(borders, topCount + 1, x, y)) {
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
