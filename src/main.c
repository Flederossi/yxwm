#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include "config.h"

int is_window(Display *dpy, Window w){
	Atom actual_type;
	int actual_format;
	unsigned long nitems, bytes_after;
	unsigned char *prop;

	if (XGetWindowProperty(dpy, w, XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False), 0, 1, False, XA_ATOM, &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success){
		if (actual_type == XA_ATOM && actual_format == 32 && nitems > 0){
			Atom window_type = ((Atom *)prop)[0];
			if (window_type == XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_NORMAL", False)){
				XFree(prop);
				return 1;
			}
		}
		XFree(prop);
	}
	return 0;
}

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
		if (attr.map_state == IsViewable && is_window(dpy, wins[i])){
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

void make_master(Display *dpy, Window w){
	XReparentWindow(dpy, w, DefaultRootWindow(dpy), 0, 0);
	XLowerWindow(dpy, w);
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

int errorHandler(Display *dpy, XErrorEvent *e){
	return 0;
}

int main(){
	Display *dpy;
	XEvent ev;
	XButtonEvent *evb;

	if (!(dpy = XOpenDisplay(NULL))) return 1;

	XSetErrorHandler(errorHandler);

	int off = 0;
	unsigned int offd = DefaultScreenOfDisplay(dpy)->width / 16;

	static int keys[] = {XK_Return, XK_space, XK_Q, XK_Left, XK_Right, XK_Escape, XK_BackSpace};
	for (int i = 0; i < sizeof(keys) / sizeof(*keys); i++) XGrabKey(dpy, XKeysymToKeycode(dpy, keys[i]), Mod1Mask, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);

	XSelectInput(dpy, DefaultRootWindow(dpy), SubstructureNotifyMask);
	
	while (!XNextEvent(dpy, &ev)){
		if (ev.type == MapNotify || ev.type == UnmapNotify) update(dpy, off);

		if (ev.type == KeyPress && ev.xkey.state == Mod1Mask){
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Return)) launch(TERMINAL);
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_space)) launch(LAUNCHER);
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Q) && ev.xkey.subwindow != None) kill(dpy, ev.xkey.subwindow);
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Left) && abs(off-offd) < DefaultScreenOfDisplay(dpy)->width / 2){ off-=offd; update(dpy, off); }
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Right) && abs(off+offd) < DefaultScreenOfDisplay(dpy)->width / 2){ off+=offd; update(dpy, off); }
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_Escape)) break;
			if (ev.xkey.keycode == XKeysymToKeycode(dpy, XK_BackSpace) && ev.xkey.subwindow != None){ make_master(dpy, ev.xkey.subwindow); update(dpy, off); }
		}
	}

	XCloseDisplay(dpy);
	return 0;
}
