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

int main(int argc, char *argv[])
{
	int fd, codRead;
	char ch;

	if (argv[1] == NULL)
	{
		fprintf(stderr, "Programul trebuie apelat cu cel putin un parametru.\n");
		exit(1);
	}

	if (-1 == (fd = open("fis.dat", O_RDWR)) )
	{
		fprintf(stderr, "Nu pot deschide fisieru fis.dat: %s\n", strerror(errno));
		exit(2);
	}

	while (0 != (codRead = read(fd, &ch, 1)) )
	{
		if (-1 == codRead)
		{
			fprintf(stderr, "errno = %d : ", errno);
			perror("Eroare la apelul read");
			exit(3);
		}

		if (ch == '#')
		{
			if (-1 == lseek(fd, -1L, 1))
			{
				perror("Eroare la apelul lseek");
				exit(4);
			}
			sleep(3);
			if (-1 == write(fd, argv[1], 1) )
			{
				perror("Eroare la apelul write");
				exit(5);
			}
		
			printf("Terminat. S-a inlocuit primul # gasit in fisierul fis.dat [ProcesId:%d].\n", getpid());
			return 0;
		}
	}	

	close(fd);
	printf("Terminat. Nu exista # in fisierul fis.dat [ProcesId:%d].\n", getpid());
	return 0;
}
