int flag;
char string[150];
char down_text[100], top_text[100];
struct udev *udev;
struct  vols{   // -- массив разделов
		int ismounted; // Монтирован ли раздел ?
		//const char *drv_name; // Название устройства
		const char *vol_name; // Название раздела
		//const char *dev_name; // Название устройства
		const char *mount_path; // Путь монтирования
		const char *mount_options; // Опции монтирования
}; vols *volumes;

/////////////////////////////////////////////////////////
int get_volumes_qty () {

char *mnt_dir, *mnt_type, *mnt_fsname, *mnt_opts;
struct mntent *mn_tab;
FILE *file = setmntent("/etc/mtab","r");
int c=0;
while (mn_tab = getmntent(file)) {
	mnt_dir = mn_tab -> mnt_dir;
    mnt_type = mn_tab -> mnt_type;
    mnt_fsname = mn_tab -> mnt_fsname;
    mnt_opts = mn_tab -> mnt_opts;
	if (strstr(mnt_fsname,"/dev/") && strcmp(mnt_dir,"/") && strcmp(mnt_dir,"/home") && strcmp(mnt_dir,"/boot")) { c++; }
}
return c;
}
/////////////////////////////////////////////////////////
void fill_volumes_array () {

char *mnt_dir, *mnt_type, *mnt_fsname, *mnt_opts;
struct mntent *mn_tab;
FILE *file = setmntent("/etc/mtab","r");
int c=0;

while (mn_tab = getmntent(file)) {
	mnt_dir = mn_tab -> mnt_dir;
    mnt_type = mn_tab -> mnt_type;
    mnt_fsname = mn_tab -> mnt_fsname;
    mnt_opts = mn_tab -> mnt_opts;
	if (strstr(mnt_fsname,"/dev/") && strcmp(mnt_dir,"/") && strcmp(mnt_dir,"/home") && strcmp(mnt_dir,"/boot")) {
			//printf("Number: %d\n", c);
			//printf("mtab: ---------------------\n");
			printf("Mount point: %s\n", mnt_dir);
			//printf("Mount FS type: %s\n", mnt_type);
			//printf("Mount FS name: %s\n", mnt_fsname);
			//printf("Mount options: %s\n", mnt_opts);
			printf("--------------------------\n");
			volumes[c].ismounted = 1;
			volumes[c].vol_name = mnt_fsname;
			volumes[c].mount_path = mnt_dir;
			volumes[c].mount_options = mnt_opts;
			c++;
	}
}
}
/////////////////////////////////////////////////////////
int drive_mount (const char *dev, mode_t mode, const char *fs_type , const char *path) {

	mkdir(path, mode);
	flag = mount(dev,path , fs_type, MS_MGC_VAL | MS_NOSUID, "");
	sprintf(string,"chmod -R 0777 %s*",path);
	system(string);
return 0;
}
/////////////////////////////////////////////////////////
int drive_unmount (const char *path ) {

	umount(path);
	rmdir(path);
return 0;
}
