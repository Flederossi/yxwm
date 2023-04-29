# ywm
A tiny (~67 loc) tiling window manager for X11 with a big focus on a lightweight, stripped down approach. It supports basic functionalities like open a terminal, format the size of the windows as well as manage the aspect ratio between them in a clean way. The windows are organized with a master window on the left and a window stack of the other windows on the right side of the screen (like the dwm standart configuration).

*Note: The wm is still experimental at this point. Therefore, I would not recommend using ywm as a window manager for everyday use due to the rather low functional scope.*

![Alt Text](https://github.com/Flederossi/ywm/blob/main/assets/screen.png)

## Installation
> Dependencies: libx11, libx11-dev
```
make install
```

## Starting ywm
To start ywm besides the XServer, you can simply add this line to the end of your .xinitrc file:
```
exec ywm
```
Then start the XServer:
```
startx
```

## Usage
| Keystroke | Description |
|-|-|
| Mod + Return | Opens a new terminal |
| Mod + Space | Opens the launcher |
| Mod + Left | Expand the width of the side stack |
| Mod + Right | Expand the width of the master window |
| Mod + Q | Closes the selected window (the window the mouse is on) |
| Mod + Esc | Quits the wm |

## Configuration
There are a few configuration options directly in the [main.c](https://github.com/Flederossi/ywm/blob/main/src/main.c) file you can change to your needs, like the terminal and launcher which are opened at the keystroke events. After configuring the variables you need to recompile ywm.
