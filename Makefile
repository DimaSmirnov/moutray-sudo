all: mountray-sudo

mountray-sudo: main.c drive.c gtk.c notify.c variables.c config.c
	g++ -g -o mountray-sudo main.c `pkg-config --cflags --libs gtk+-2.0` -lnotify -ludev

