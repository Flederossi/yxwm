build:
	gcc src/*.c -o bin/yxwm -lm -lX11
install: build
	cp bin/yxwm /usr/bin/yxwm
uninstall:
	rm /usr/bin/yxwm
