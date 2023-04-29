#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

static unsigned int BORDER = 2;
static unsigned int BORDER_COLOR = 0x888888;

static char *TERMINAL[] = {"xterm", NULL};
static char *LAUNCHER[] = {"dmenu_run", NULL};

void update(Display *dpy, int off){
	Window root, parent, *wins;
	XWindowAttributes attr;
	unsigned int n;
	unsigned int ww = DefaultScreenOfDisplay(dpy)->width;
	unsigned int wh = DefaultScreenOfDisplay(dpy)->height;

	XQueryTree(dpy, DefaultRootWindow(dpy), &root, &parent, &wins, &n);

	if (n == 1) XMoveResizeWindow(dpy, wins[0], 0, 0, ww - BORDER * 2, wh - BORDER * 2);
	else if (n > 1) {
		XMoveResizeWindow(dpy, wins[0], 0, 0, ww / 2 + off, wh - BORDER * 2);
		for (int i = 1; i < n; i++) XMoveResizeWindow(dpy, wins[i], ww / 2 + off, wh / (n - 1) * (i - 1), ww / 2 - off - BORDER * 2, i == n - 1 ? wh / (n - 1) - BORDER * 2 : wh / (n - 1));
	}

	for (int i = 0; i < n; i++){
		XSetWindowBorderWidth(dpy, wins[i], BORDER);
		XSetWindowBorder(dpy, wins[i], BORDER_COLOR);
	}

	if (wins) XFree(wins);
}

void launch(char **args){
	if (fork() == 0){
		setsid();
		execvp(args[0], args);
		exit(0);
	}
}

void kill(Display *dpy, Window w){
	XEvent ev;
	ev.type = ClientMessage;
	ev.xclient.window = w;
	ev.xclient.message_type = XInternAtom(dpy, "WM_PROTOCOLS", True);
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	ev.xclient.data.l[1] = CurrentTime;
	XSendEvent(dpy, w, False, NoEventMask, &ev);
}

int main(){
	Display *dpy;
	XEvent ev;
	XButtonEvent *evb;

	if (!(dpy = XOpenDisplay(NULL))) return 1;

	int off = 0;
	unsigned int offd = DefaultScreenOfDisplay(dpy)->width / 16;

	static int keys[] = {XK_Return, XK_space, XK_Q, XK_Left, XK_Right, XK_Escape};
	for (int i = 0; i < sizeof(keys) / sizeof(*keys); i++) XGrabKey(dpy, XKeysymToKeycode(dpy, keys[i]), Mod1Mask, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);

	XSelectInput(dpy, DefaultRootWindow(dpy), SubstructureNotifyMask);
	
	while (!XNextEvent(dpy, &ev)){
		if (ev.type == MapNotify || ev.type == UnmapNotify) update(dpy, off);

		if (ev.type == KeyPress){
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Return) && ev.xkey.state == Mod1Mask) launch(TERMINAL);
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_space) && ev.xkey.state == Mod1Mask) launch(LAUNCHER);
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Q) && ev.xkey.state == Mod1Mask && ev.xkey.subwindow != None) kill(dpy, ev.xkey.subwindow);
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Left) && ev.xkey.state == Mod1Mask && abs(off-offd) < DefaultScreenOfDisplay(dpy)->width / 2){ off-=offd; update(dpy, off); }
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Right) && ev.xkey.state == Mod1Mask && abs(off+offd) < DefaultScreenOfDisplay(dpy)->width / 2){ off+=offd; update(dpy, off); }
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Escape) && ev.xkey.state == Mod1Mask) break;
		}
	}

	XCloseDisplay(dpy);
	return 0;
}
