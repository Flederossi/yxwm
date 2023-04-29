build:
	gcc src/*.c -o bin/ywm -lX11
install: build
	cp bin/ywm /usr/bin/ywm
uninstall:
	rm /usr/bin/ywm
