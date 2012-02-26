#include "libudev.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <mntent.h>

#define MOUNT_PATH "/media/"

#include "variables.c"
#include "notify.c"
#include "drive.c"
#include "gtk.c"


int main( int argc, char *argv[] ) {

	int gid = getuid ();
	if (gid) { Create_notify("Please run program as root!", "Mountray-sudo", 3000); printf("Please run program as root!\n"); return -1; }


	GtkWidget *response_widget;
	gtk_init(&argc, &argv);

    udev = udev_new();
    mon = udev_monitor_new_from_netlink( udev, "udev" );
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", NULL);
	udev_monitor_enable_receiving(mon);
	fd = udev_monitor_get_fd(mon);
    FD_ZERO(&readfds);

    g_timeout_add(1000, (GSourceFunc) TimerFuncReturnStatusIcon, response_widget);
    g_timeout_add(250, (GSourceFunc) ReadUdevStat, response_widget);

	tray_icon = CreateTrayIcon();

 	gtk_main();

return 0;
}
