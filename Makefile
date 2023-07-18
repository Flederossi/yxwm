build:
	gcc src/*.c -o bin/yxwm -I src/include -lm -lX11
install: build
	cp bin/yxwm /bin/yxwm
uninstall:
	rm /bin/yxwm
