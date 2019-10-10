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
			perror("Eroare la crearea pipe ului hi2ho");
			exit(1);
		}
	}
	if (-1 == mkfifo("ho2hi",0600))
	{
		if (errno != EEXIST)
		{
			perror("Eroare la crearea pipe ului ho2hi");
			exit(2);
		}
	}

	fdi = open("hi2ho",O_RDONLY);
	fdo = open("ho2hi",O_WRONLY);

	while(nr--)
	{
		read(fdi,&ch,1);
		printf("ho, ");
		fflush(stdout);
		write(fdo,&ch,1);
	}

	printf("\n Sfarsit hiho!\n");
	return 0;
}
