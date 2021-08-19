#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>

#include "levels.h"

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
	if (keysPressed[UP_PRESSED] && *y == height - 10 && *ySpeed == 0)
		*ySpeed = -15;

	*y = *y + *ySpeed;
	if (*y < height - 10)
		*ySpeed += 1;
}
