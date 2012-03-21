fd_set readfds;
int fd, ret, volumes_qty;
int iconchangestatus=0;
void Create_menu();

struct udev_monitor *mon;
char device_name[100], path[100];
GtkStatusIcon *tray_icon;
GtkWidget *menu, *menuitem, *widget, *image, *sep;
GtkIconSize icon_size;

/////////////////////////////////////
void ActionOnQuit() {
	printf("Exiting. See you again!\n");
	gtk_widget_destroy (menu);
	gtk_main_quit ();
	free(volumes);
}
/////////////////////////////////////
void ActionOnDriveClick(GtkWidget *widget, GdkEvent *event) {
	const gchar *name = gtk_widget_get_name(widget);
	for (int i=0;i<volumes_qty;i++) {
		if (!strcmp(volumes[i].vol_name,name)) {
			gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-active.png");
			iconchangestatus=1;
			drive_unmount (volumes[i].mount_path); /////////////
			sprintf(top_text,"Device %s unmounted", volumes[i].mount_path);
			volumes[i].ismounted = 0;
			sprintf(down_text,"And can be safety removed");
			Create_notify(down_text, top_text, 3000);
			free(volumes);
			gtk_widget_destroy (menu);
			Create_menu();
		}
	}
}
/////////////////////////////////////
void AddDrivesToMenu(GdkPixbuf *pixbuf, GdkPixbuf *pixbuf2) {
	for (int i=0;i<volumes_qty;i++) { // Add drives to menu
		if (volumes[i].ismounted) {
			image = gtk_image_new_from_pixbuf(pixbuf);
			menuitem = gtk_image_menu_item_new_with_label(volumes[i].mount_path);
			gtk_widget_set_tooltip_text (menuitem,"Volume mounted");
		}
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM (menuitem), image);
		gtk_widget_set_name(menuitem, volumes[i].vol_name);
		g_signal_connect(menuitem, "activate",(GCallback) ActionOnDriveClick,  menuitem);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	}
}
/////////////////////////////////////
void Create_menu() {
	menu = gtk_menu_new();
	volumes_qty = get_volumes_qty ();
	volumes = new vols[volumes_qty];
	if (volumes_qty) {
		//for (int i=0;i<volumes_qty;i++) {
		//	if (!strcmp(volumes[i].mount_path,path)) volumes[i].drv_name=vol_label;
		//}
		menuitem = gtk_menu_item_new_with_label ("Volumes:");
		gtk_widget_set_sensitive (menuitem,FALSE);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
		GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("/usr/share/pixmaps/mountray/unmount.png", &error);
		GdkPixbuf *pixbuf2 = gdk_pixbuf_new_from_file("/usr/share/pixmaps/mountray/drive-normal.png", &error);
		fill_volumes_array();
		AddDrivesToMenu(pixbuf, pixbuf2);
		sep = gtk_separator_menu_item_new(); // сепаратор
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), sep);
	}
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
	g_signal_connect(menuitem, "activate",(GCallback) ActionOnQuit,  widget);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
}
/////////////////////////////////////////////////
gboolean ReadUdevStat(GtkWidget *label) { // Чтение информации о устройстве, которое подключается/отключается.
		struct udev_device *device;
		struct timeval tv;

		FD_SET(STDIN_FILENO, &readfds);
        FD_SET(fd, &readfds);
          /* Interruption Delay. */
		tv.tv_sec = 0;
		tv.tv_usec = 250;
		ret = select(fd+1, &readfds, NULL, NULL, &tv);
		if (ret > 0) {
			device = udev_monitor_receive_device(mon);
			const char *dev  = udev_device_get_devnode(device);
			const char *dev_type  = udev_device_get_property_value(device,"DEVTYPE");
			const char *fs_type  = udev_device_get_property_value(device,"ID_FS_TYPE");
			const char *vol_label  = udev_device_get_property_value(device,"ID_FS_LABEL");
			const char *action = udev_device_get_action(device);
			if (!strcmp(dev_type,"partition")) {
				strcpy(device_name, udev_device_get_property_value(device,"ID_VENDOR"));
				strcat (device_name," ");
				strcat (device_name, udev_device_get_property_value(device,"ID_MODEL"));
				//printf("DEBUG: Volume name - '%s'\n",vol_label);
				if (!vol_label) vol_label="Incorrect_volume_name";

				if (!strcmp(action,"add")) { // if connect
					mode_t mode=0777;
					strcpy(path, MOUNT_PATH);
					strcat(path,vol_label);
					gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-active.png");
					iconchangestatus=1;
					int a = drive_mount(dev,mode,fs_type, path); // mounting
					sprintf(top_text,"%s mounted", vol_label);
					sprintf(down_text,"Path: %s", path);
					Create_notify(down_text, top_text, 3000);
					free(volumes);
					gtk_widget_destroy (menu);
					Create_menu();
				}
				else { // if unmounting, without disconnect
					gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-active.png");
					iconchangestatus=1;
					int a = drive_unmount(path);  // UNmounting
					sprintf(down_text,"Device %s removed", vol_label);
					sprintf(top_text,"Mountray");
					Create_notify(down_text, top_text, 3000);
					free(volumes);
					gtk_widget_destroy (menu);
					Create_menu();
				}
			}
			/////////////
			if (!strcmp(action,"change") && !strcmp(dev_type,"disk") ) { // if remove, when mounted
				for (int i=0;i<volumes_qty;i++) {
					printf("DEBUG: Path - '%s' \n", path);
					if (!strcmp(path, volumes[i].mount_path) && volumes[i].ismounted) {
						gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-active.png");
						iconchangestatus=1;
						int a = drive_unmount(path);  // UNmounting
						sprintf(top_text,"Device '%s' is hard removed", volumes[i].vol_name);
						sprintf(down_text,"Please check it for errors");
						Create_notify(down_text, top_text, 3000);
						free(volumes);
						gtk_widget_destroy (menu);
						Create_menu();
						break;
					}
				}
			}
			///////////////
			udev_device_unref(device);
		}
	return TRUE;
}
/////////////////////////////////////
void ShowTrayMenu(GtkWidget *widget, GdkEvent *event) {
	gtk_widget_show_all(menu);
	gtk_menu_popup (GTK_MENU(menu), NULL, NULL, NULL, NULL, NULL, NULL);
}

/////////////////////////////////////////////////
static GtkStatusIcon *CreateTrayIcon() {
	tray_icon = gtk_status_icon_new();
	g_signal_connect(G_OBJECT(tray_icon), "activate", G_CALLBACK(ShowTrayMenu), NULL);
	g_signal_connect_swapped(G_OBJECT(tray_icon), "popup-menu",G_CALLBACK(ShowTrayMenu), NULL);
	gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-normal.png");
	gtk_status_icon_set_tooltip(tray_icon, "Mountray. Tray mounter");
	gtk_status_icon_set_visible(tray_icon, TRUE);
	Create_menu();
	return tray_icon;
}
/////////////////////////////////////
gboolean TimerFuncReturnStatusIcon(GtkWidget *label) {
	 if (iconchangestatus) {
		gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-normal.png");
		iconchangestatus=0;
	}
	return TRUE;
}
