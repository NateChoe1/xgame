#include <unistd.h>
#include <X11/Xlib.h>

#include "help.h"
#include "levels.h"

void level1(Display *dpy, Window root, Window win, Window player) {
	int windowX, windowY, x, y, ySpeed, width, height;
	char keysPressed[TOTAL_CONTROLS] = {0};
	Window unusedWindow;
	int unusedInt;
	//used in functions which require a pointer but which I don't want.

	XTranslateCoordinates(dpy, win, root, 0, 0,
			&windowX, &windowY, &unusedWindow);
	moveResizeWindow(dpy, root, win, 300, 300, 500, 400);
	XFlush(dpy);
	for (;;) {
		int oldWindowX = windowX;
		int oldWindowY = windowY;
		XTranslateCoordinates(dpy, win, root, 0, 0,
				&windowX, &windowY, &unusedWindow);
		if (windowX != oldWindowX || windowY != oldWindowY)
			break;
	}
	//I have no idea why, but XSync doesn't work for this garbage on dwm, so I'm
	//just doing this crap.
	
	x = 250;
	y = 390;
	ySpeed = 0;

	int wmXOffset, wmYOffset;
	//Some window managers (dwm) will move the window to the wrong place by an
	//offset. This is to correct for that.

	XTranslateCoordinates(dpy, win, root, 0, 0,
			&windowX, &windowY, &unusedWindow);
	wmXOffset = windowX - 300;
	wmYOffset = windowY - 300;

	for (;;) {
		XRaiseWindow(dpy, win);
		XRaiseWindow(dpy, player);
		updateKeysPressed(dpy, keysPressed);
		updatePosition(dpy, keysPressed, width, height,
				&x, &y, &ySpeed);
		int oldWindowX = windowX;
		int oldWindowY = windowY;
		XTranslateCoordinates(dpy, win, root, 0, 0,
				&windowX, &windowY, &unusedWindow);
		XGetGeometry(dpy, win, &unusedWindow, &unusedInt, &unusedInt,
				&width, &height,
				&unusedInt, &unusedInt);

		char windowMoved = 0;
		if (y > height - 10 || y == height - 10 && ySpeed > 0) {
			y = height - 10;
			ySpeed = 0;
			windowY += 10;
			windowMoved = 1;
		}
		if (x < 0) {
			windowX += x;
			x = 0;
			windowMoved = 1;
		}
		if (x + 10 > width) {
			windowX += width - x;
			x = width - 10;
			windowMoved = 1;
		}

		if (!windowMoved) {
			x -= windowX - oldWindowX;
			y -= windowY - oldWindowY;
			if (x < 0)
				x = 0;
			if (x > width - 10)
				x = width - 10;
			if (y > height - 10)
				y = height - 10;
		}
		else
			moveResizeWindow(dpy, root, win,
					windowX - wmXOffset, windowY - wmYOffset, width, height);
		if (y < 0)
			break;
		XMoveWindow(dpy, player, windowX + x, windowY + y);

		usleep(16666);
		XSync(dpy, False);
	}

	return;
}
