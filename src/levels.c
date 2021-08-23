#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>

#include "levels.h"
#include "help.h"

int wmXOffset, wmYOffset;
//moveResizeWindow may be off due to the specifics of how your window manager
//works, these variables store that offset.

void updateKeysPressed(Display *dpy, char keysPressed[TOTAL_CONTROLS]) {
	int pending = XPending(dpy);
	XEvent e;
	for (int i = 0; i < pending; i++) {
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
}

void updatePosition(Display *dpy, char keysPressed[3], int width, int height,
		int *x, int *y, int *ySpeed) {
	if (keysPressed[LEFT_PRESSED])
		*x -= 5;
	if (keysPressed[RIGHT_PRESSED])
		*x += 5;
	*y = *y + *ySpeed;
}

void initVars(Display *dpy, Window root, Window win) {
	Window unusedWindow;
	int windowX, windowY;
	wmXOffset = wmYOffset = 0;
	int oldWindowX, oldWindowY;
	XTranslateCoordinates(dpy, win, root, 0, 0,
			&windowX, &windowY, &unusedWindow);
	moveResizeWindow(dpy, root, win, START_X, START_Y,
			WINDOW_WIDTH, WINDOW_HEIGHT);
	//these values are arbitrary.
	waitForResize(dpy, root, win, windowX, windowY);

	XTranslateCoordinates(dpy, win, root, 0, 0,
			&wmXOffset, &wmYOffset, &unusedWindow);

	wmXOffset -= START_X;
	wmYOffset -= START_Y;
}
