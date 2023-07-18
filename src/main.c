#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "config.h"

void update(Display *dpy, int off){
	Window root, parent, *wins;
	unsigned int n;
	unsigned int ww = DefaultScreenOfDisplay(dpy)->width;
	unsigned int wh = DefaultScreenOfDisplay(dpy)->height;

	XQueryTree(dpy, DefaultRootWindow(dpy), &root, &parent, &wins, &n);

	Window *vis_wins = malloc(0);
	unsigned int rn = 0;
	XWindowAttributes attr;

	for (int i = 0; i < n; i++){
		XGetWindowAttributes(dpy, wins[i], &attr);
		if (attr.map_state == IsViewable){
			XSetWindowBorderWidth(dpy, wins[i], BORDER);
			XSetWindowBorder(dpy, wins[i], BORDER_COLOR);

			rn++;
			vis_wins = realloc(vis_wins, sizeof(Window) * rn);
			vis_wins[rn - 1] = wins[i];
		}
	}

	if (rn == 1) XMoveResizeWindow(dpy, vis_wins[0], GAPS * SINGLE_GAPS, GAPS * SINGLE_GAPS, ww - BORDER * 2 - GAPS * SINGLE_GAPS * 2, wh - BORDER * 2 - GAPS * SINGLE_GAPS * 2);
	else if (rn > 1) {
		XMoveResizeWindow(dpy, vis_wins[0], GAPS, GAPS, ww / 2 + off - 2 * BORDER - GAPS, wh - BORDER * 2 - 2 * GAPS);
		for (int i = 1; i < rn; i++)
			XMoveResizeWindow(dpy, vis_wins[i], ww / 2 + off + GAPS, ceil(wh / (rn - 1)) * (i - 1) + GAPS, ww / 2 - off - 2 * BORDER - 2 * GAPS, i == rn - 1 ? ceil(wh / (rn - 1)) - 2 * BORDER - 2 * GAPS : ceil(wh / (rn - 1)) - 2 * BORDER - GAPS);
	}
	
	if (wins) XFree(wins);
	XFree(vis_wins);
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
