.POSIX:

PREFIX = /usr/local
CC = gcc
INCS = -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include
LIBS = -lnotify -lgdk_pixbuf-2.0 -lglib-2.0

asus-fan-daemon: main.o
	$(CC) main.o $(LIBS) -o asus-fan-daemon
main.o: main.c
	$(CC) -c main.c $(INCS)
clean:
	rm -f *.o asus-fan-daemon
install: asus-fan-daemon
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f asus-fan-daemon $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/asus-fan-daemon
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/asus-fan-daemon

.PHONY: clean install uninstall
