# yxwm
A tiny (~80 loc) tiling window manager for X11 with a big focus on a lightweight, stripped down environment. It supports basic functionalities like open a terminal or a launcher, format the size of the windows as well as manage the aspect ratio between them in a clean way. The windows are organized with a master window on the left and a window stack of the other windows on the right side of the screen (like the dwm standard configuration).

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
| Mod + Return | Open a new terminal |
| Mod + Space | Open the launcher |
| Mod + Left-Arrow | Expand the width of the side stack |
| Mod + Right-Arrow | Expand the width of the master window |
| Mod + Backspace | Set the selected window (the window the mouse is on) as master window |
| Mod + Q | Close the selected window |
| Mod + Esc | Quit the wm |

<br />

## Configuration
You can configure yxwm in the [config.h](https://github.com/Flederossi/yxwm/blob/main/src/include/config.h) file. Every option is described with a small comment. After configuring the variables, yxwm should be recompiled.
