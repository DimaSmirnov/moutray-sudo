all: mountray-sudo

mountray-sudo: main.c drive.c gtk.c notify.c variables.c config.c
	g++ -g -o mountray-sudo main.c `pkg-config --cflags --libs gtk+-2.0` -lnotify -ludev

install: mountray-sudo
	cp mountray-sudo /usr/bin/mountray-sudo
	cp icons/drive-active-big.png /usr/share/pixmaps/mountray/drive-active-big.png
	cp icons/drive-active.png /usr/share/pixmaps/mountray/drive-active.png
	cp icons/drive-normal.png /usr/share/pixmaps/mountray/drive-normal.png
	cp icons/unmount.png /usr/share/pixmaps/mountray/unmount.png

uninstall: mountray-sudo
	rm /usr/bin/mountray-sudo
	rm /usr/share/pixmaps/mountray/drive-active-big.png
	rm /usr/share/pixmaps/mountray/drive-active.png
	rm /usr/share/pixmaps/mountray/drive-normal.png
	rm /usr/share/pixmaps/mountray/unmount.png
