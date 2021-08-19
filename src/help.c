#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "help.h"

#define MAX_READ_ATOMS 64

#define MOVERESIZE_WINDOW (1 << 0)

int supportedHints = 0;

Atom atoms[ATOM_COUNT];
Atom *supportedExtensions;
int extensionCount;

char initAtoms(Display *dpy, Window root) {
	atoms[_NET_SUPPORTED] = XInternAtom(dpy, "_NET_SUPPORTED", False);
	atoms[_NET_MOVERESIZE_WINDOW] = XInternAtom(dpy, "_NET_MOVERESIZE_WINDOW", False);
	atoms[_NET_WM_WINDOW_TYPE] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
	atoms[_NET_WM_WINDOW_TYPE_DIALOG] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
	atoms[_NET_WM_WINDOW_TYPE_NORMAL] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	atoms[_NET_WM_NAME] = XInternAtom(dpy, "_NET_WM_NAME", False);
	//this breaks the 80 character rule, but I think it's probably fine.

	Atom type;
	int format;
	unsigned long atomCount, remainingBytes;

	int allocatedExtensions = 16;
	supportedExtensions = malloc(allocatedExtensions * sizeof(Atom));
	extensionCount = 0;
	for (;;) {
		Atom type;
		int format;
		unsigned long readItems, toBeRead;
		Atom *newAtoms;
		XGetWindowProperty(dpy, root, atoms[_NET_SUPPORTED],
				extensionCount * sizeof(Atom) / 4,
				(allocatedExtensions - extensionCount) * sizeof(Atom) / 4,
				False, XA_ATOM,
				&type, &format, &readItems, &toBeRead,
				(unsigned char **) &newAtoms
		);
		for (int i = 0; i < readItems; i++)
			supportedExtensions[i + extensionCount] = newAtoms[i];
		extensionCount += readItems;
		if (toBeRead != 0) {
			Atom *newExtensions = malloc(allocatedExtensions * sizeof(Atom) * 2);
			for (int i = 0; i < allocatedExtensions; i++)
				newExtensions[i] = supportedExtensions[i];
			allocatedExtensions *= 2;
			free(supportedExtensions);
			supportedExtensions = newExtensions;
			continue;
		}
		break;
	}
}

char extensionSupported(int extension) {
	for (int i = 0; i < extensionCount; i++) {
		if (supportedExtensions[i] == atoms[extension])
			return 1;
	}
	return 0;
}

char moveResizeWindow(Display *dpy,
		Window root, Window win,
		int x, int y, int w, int h) {
	if (extensionSupported(_NET_MOVERESIZE_WINDOW)) {
		XClientMessageEvent event = {
			.type = ClientMessage,
			.serial = 0,
			.send_event = True,
			.display = dpy,
			.window = win,
			.message_type = atoms[_NET_MOVERESIZE_WINDOW],
			.format = 32,
			.data.l[0] = 0,
			.data.l[1] = x,
			.data.l[2] = y,
			.data.l[3] = w,
			.data.l[4] = h,
		};

		if (XSendEvent(dpy, root, False, 0, (XEvent *) &event))
			return 1;
		return 0;
	}
	XMoveResizeWindow(dpy, win, x, y, w, h);
	return 0;
	//this function will almost definitely break on a lot of multi monitor
	//setups, and also a lot of single monitor setups, and also a lot of setups.
}

void setProperty(Display *dpy, Window win,
		Atom property, Atom *value, int count) {
	XChangeProperty(dpy, win, property, XA_ATOM, 32, PropModeReplace,
			(unsigned char *) value, count);
}
