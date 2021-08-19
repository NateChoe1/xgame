#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#include "help.h"

#define UP_PRESSED 0
#define RIGHT_PRESSED 1
#define LEFT_PRESSED 2
#define TOTAL_CONTROLS 3

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
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			if (e.type != KeyPress && e.type != KeyRelease)
				continue;
			switch (XkbKeycodeToKeysym(dpy, e.xkey.keycode, 0, e.xkey.state)) {
				case XK_Up:
					keysPressed[UP_PRESSED] = e.type == KeyPress;
					break;
				case XK_Right:
					keysPressed[RIGHT_PRESSED] = e.type == KeyPress;
					break;
				case XK_Left:
					keysPressed[LEFT_PRESSED] = e.type == KeyPress;
					break;
			}
		}
		int oldWindowX = windowX;
		int oldWindowY = windowY;
		XTranslateCoordinates(dpy, win, root, 0, 0,
				&windowX, &windowY, &unusedWindow);
		XGetGeometry(dpy, win, &unusedWindow, &unusedInt, &unusedInt,
				&width, &height,
				&unusedInt, &unusedInt);

		if (keysPressed[LEFT_PRESSED])
			x -= 5;
		if (keysPressed[RIGHT_PRESSED])
			x += 5;
		if (keysPressed[UP_PRESSED] && y == height - 10 && ySpeed == 0)
			ySpeed = -15;
		y += ySpeed;
		if (y < height - 10)
			ySpeed += 1;

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
