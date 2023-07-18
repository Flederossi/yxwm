#ifndef CONFIG_H
#define CONFIG_H

// The size of the borders of each window
static unsigned int BORDER = 3;
// The color of the borders of each window
static unsigned int BORDER_COLOR = 0x888888;

// The size of the gaps between the windows
static unsigned int GAPS = 20;
// Should gaps be applied when only one window is opened (0: False, 1: True)
static unsigned int SINGLE_GAPS = 1;

// The default terminal which opens on Mod + Return
static char *TERMINAL[] = {"xterm", "-fg", "white", "-bg", "black", "-fn", "-*-*-*-*-*-*-18-*-*-*-*-*-*-*", NULL};
// The default launcher which opens on Mod + Space
static char *LAUNCHER[] = {"dmenu_run", NULL};

#endif
