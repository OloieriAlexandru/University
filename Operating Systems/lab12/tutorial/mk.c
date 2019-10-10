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
	if (argc < 2)
	{
		fprintf(stderr, "Sintaxa apel: mk nume_fifo\n");
		exit(1);
	}

	if (-1 == mkfifo(argv[1], 0600))
	{
		if (errno == EEXIST)
		{
			fprintf(stdout, "Observatie: fisierul fifo %s exista deja!\n", argv[1]);
		}
		else
		{
			fprintf(stderr, "Eroare: creare fisier fifo imposibila, errno=%d\n", errno);
			perror("Cauza erorii este");
			exit(2);
		}
	}
	return 0;
}
