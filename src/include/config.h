#ifndef CONFIG_H
#define CONFIG_H

static unsigned int BORDER = 3;
static unsigned int BORDER_COLOR = 0x888888;

static unsigned int GAPS = 20;
static unsigned int SINGLE_GAPS = 1;

static char *TERMINAL[] = {"xterm", "-fg", "white", "-bg", "black", "-fn", "-*-*-*-*-*-*-18-*-*-*-*-*-*-*", NULL};
static char *LAUNCHER[] = {"dmenu_run", NULL};

#endif
