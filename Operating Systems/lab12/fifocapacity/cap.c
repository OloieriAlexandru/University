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
	if (-1 == mkfifo("pipe",0600))
	{
		if (errno != EEXIST)
		{
			perror("Eroare la crearea pipe ului");
			exit(1);
		}
	}

	int fd = open("pipe", O_RDWR | O_NONBLOCK);
	if (fd == -1)
	{
		perror("Eroare la deschiderea pipe ului");
		exit(2);
	}

	int codW, ans = 0;
	char ch = 0;
	while (1)
	{
		codW = write(fd, &ch, 1);
		if (codW == -1)
		{
			break;
		}
		++ans;
	}
	printf("Capacitatea unui pipe extern: %d\n", ans);
	close(fd);
	return 0;
}
