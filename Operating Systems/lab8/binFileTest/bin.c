#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{	
	int fd = open("fis.bin",O_RDONLY);
	float val;
	read(fd, &val,sizeof(val));
	printf("%f\n", val);
	close(fd);
	return 0;
}
