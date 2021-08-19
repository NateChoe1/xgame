#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "help.h"

void level1(Display *dpy, Window root, Window win, Window player);

void setWindowName(Display *dpy, Window win, char *windowName) {
	XTextProperty prop;
	Xutf8TextListToTextProperty(dpy, &windowName, 1, XUTF8StringStyle, &prop);
	XSetTextProperty(dpy, win, &prop, atoms[_NET_WM_NAME]);
}

int main() {
	Display *dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fprintf(stderr, "Couldn't open display.\n");
		exit(EXIT_FAILURE);
	}

	int screen = DefaultScreen(dpy);
	Colormap colorMap = DefaultColormap(dpy, screen);

	Window root = DefaultRootWindow(dpy);

	long black = XBlackPixel(dpy, screen);
	long white = XWhitePixel(dpy, screen);
	initAtoms(dpy, root);

	Window win = XCreateSimpleWindow(dpy, root,
			0, 0, 500, 400, 0, black, white);
	XSelectInput(dpy, win, KeyPressMask | KeyReleaseMask | ExposureMask);
	//create a basic window

	Atom type[] = {
		atoms[_NET_WM_WINDOW_TYPE_DIALOG],
		atoms[_NET_WM_WINDOW_TYPE_NORMAL],
	};
	setProperty(dpy, win, atoms[_NET_WM_WINDOW_TYPE],
			type, 2);
	//make it floating on tiling window managers

	setWindowName(dpy, win, "xgame");

	XMapWindow(dpy, win);
	for (;;) {
		XEvent e;
		XNextEvent(dpy, &e);
		if (e.type == Expose)
			break;
	}
	//map and wait for map

	XColor red = {
		.red = 0xffff,
		.flags = DoRed,
	};
	XAllocColor(dpy, colorMap, &red);

	Window player = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
			100, 100, 10, 10, 0, black, red.pixel);
	XSelectInput(dpy, win, KeyPressMask | KeyReleaseMask | ExposureMask);
	XSetWindowAttributes attr = {
		.override_redirect = True,
	};
	XChangeWindowAttributes(dpy, player, CWOverrideRedirect, &attr);
	XMapWindow(dpy, player);

	level1(dpy, root, win, player);
}
