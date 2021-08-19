#include <unistd.h>
#include <X11/Xlib.h>

#include "help.h"
#include "levels.h"

void level2(Display *dpy, Window root, Window win, Window player) {
	Window unusedWindow;

	Window *topLevels;
	unsigned int topCount;
	char keysPressed[3] = {0};

	for (;;) {
		XQueryTree(dpy, root, &unusedWindow, &unusedWindow, &topLevels, &topCount);
		updateKeysPressed(dpy, keysPressed);

		usleep(16666);
	}
}
