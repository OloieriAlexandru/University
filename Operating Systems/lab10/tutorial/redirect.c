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
	char *str1="Primul mesaj; va aparea pe ecran.\n";
	write(1,str1,strlen(str1));

	int fd;
	fd = creat("fis2.txt",0600);
	if (fd == -1)
	{
		perror("Deschiderea fisierului a esuat. Motivul:");
		exit(2);
	}
	close(1);
	if (-1 == dup2(fd, 1))
	{
		perror("Redirectarea a esuat. Motivul:");
		exit(1);
	}
	close(fd);

	char *str2="Al doilea mesaj scris pe stdout, nu va aparea pe ecran, ci in fisier.\n";
	write(1, str2, strlen(str2));

	int fd1;
	fd1 = open("/dev/tty",O_WRONLY);
	if (fd1 == -1)
	{
		perror("Deschiderea terminalului a esuat. Motivul:");
		exit(2);
	}
	close(1);
	if (-1 == dup2(fd1, 1))
	{
		perror("Re-directarea a esuat. Motivul:");
		exit(3);
	}
	close(fd1);
	char *str3="Al treilea mesaj, va aparea pe ecran.\n";
	write(1, str3, strlen(str3));

	return 0;
}
