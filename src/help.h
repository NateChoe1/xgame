char initAtoms(Display *dpy, Window root);
char extensionSupported(int extension);
char moveResizeWindow(Display *dpy,
		Window root, Window win,
		int x, int y, int w, int h);
void setProperty(Display *dpy, Window win,
		Atom property, Atom *value, int count);

#define _NET_SUPPORTED 0
#define _NET_MOVERESIZE_WINDOW 1
#define _NET_WM_WINDOW_TYPE 2
#define _NET_WM_WINDOW_TYPE_DIALOG 3
#define _NET_WM_WINDOW_TYPE_NORMAL 4
#define _NET_WM_NAME 5
#define ATOM_COUNT 6
extern Atom atoms[ATOM_COUNT];

extern Atom *supportedExtensions;
extern int extensionCount;
