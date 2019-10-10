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
	int fdi, fdo, nr = 100;
	char ch = '0';
	if (-1 == mkfifo("hi2ho",0600))
	{
		if (errno != EEXIST)
		{
			perror("Eroare la crearea hi2ho");
			exit(1);
		}
	}

	fdo = open("hi2ho",O_WRONLY);
	fdi = open("ho2hi",O_RDONLY);
	while (nr--)
	{
		printf("Hi-");
		fflush(stdout);
		write(fdo,&ch,1);
		read(fdi,&ch,1);
	}
	return 0;
}
