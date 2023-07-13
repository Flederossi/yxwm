# yxwm
A tiny (~67 loc) tiling window manager for X11 with a big focus on a lightweight, stripped down environment. It supports basic functionalities like open a terminal, format the size of the windows as well as manage the aspect ratio between them in a clean way. The windows are organized with a master window on the left and a window stack of the other windows on the right side of the screen (like the dwm standard configuration).

*Note: The wm is still experimental at this point. Therefore, I would not recommend using yxwm as a window manager for everyday use due to the rather low functional scope.*

<br />

![Alt Text](https://github.com/Flederossi/ywm/blob/main/assets/screen.png)

<br />

## Installation
> Dependencies: libx11, libx11-dev
```
mkdir bin
make install
```

## Start yxwm
> Add this line to the end of the .xinitrc file on your system, to start yxwm besides the XServer:
```
exec yxwm
```
> Then start the XServer:
```
startx
```

## Usage
| Keystroke | Description |
|-|-|
| Mod + Return | Opens a new terminal |
| Mod + Space | Opens the launcher |
| Mod + Left-Arrow | Expand the width of the side stack |
| Mod + Right-Arrow | Expand the width of the master window |
| Mod + Q | Closes the selected window (the window the mouse is on) |
| Mod + Esc | Quits the wm |

<br />

## Configuration
There are a few configuration options directly in the [main.c](https://github.com/Flederossi/yxwm/blob/main/src/main.c) file which can be changed as needed, such as the terminal and launcher used on the specific keystroke event or the border settings. After configuring the variables, yxwm should be recompiled.
