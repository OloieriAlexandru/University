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

void bubble_sort(int fileDescr);

int main(int argc, char *argv[])
{	
	int fd;
	if (argc < 2)
	{
		printf("Usage: %s datafile\n", argv[0]);
		exit(1);
	}
	if (-1 == (fd = open(argv[1], O_RDWR)))
	{
		perror("Eroare la deschiderea fisierului de date");
		exit(2);
	}
	bubble_sort(fd);
	close(fd);
	printf("Notificare: [PID: %d] Am terminat (contributia mea) la sortarea fisierului %s !\n", getpid(), argv[1]);
	return 0;
}

void bubble_sort(int fileDescr)
{
	int rcod1, rcod2;
	int numar1, numar2;
	int modificare = 1;
	while(modificare)
	{
		modificare = 0;

		while(1)
		{
			rcod1 = read(fileDescr, &numar1, sizeof(int));
			if (rcod1 == 0)
			{
				break;
			}
			if (rcod1 == -1)
			{
				perror("Eroare la citirea primului numar dintr-o pereche");
				exit(3);
			}
			rcod2 = read(fileDescr, &numar2, sizeof(int));
			if (rcod2 == 0)
			{
				break;
			}
			if (rcod2 == -1)
			{
				perror("Eroare la citirea celui de-al doilea numar dintr-o pereche");
				exit(4);
			}
			if (numar1 > numar2)
			{
				modificare = 1;
				if (-1 == lseek(fileDescr, -2*sizeof(int), SEEK_CUR))
				{
					perror("Eroare (1) la repozitionarea inapoi in fisier");
					exit(5);
				}
				if(-1 == write(fileDescr, &numar2, sizeof(int)))
				{
					perror("Eroare la rescrierea primului numar dintr-o pereche");
					exit(6);
				}
				if(-1 == write(fileDescr, &numar1, sizeof(int)))
				{
					perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche");
					exit(7);
				}
			}
			if (-1 == lseek(fileDescr, -sizeof(int), SEEK_CUR))
			{
				perror("Eroare (2) la repozitionarea inapoi in fisier");
				exit(8);
			}
		}
		if (-1 == lseek(fileDescr, 0L, SEEK_SET))
		{
			perror("Eroare (3) la repozitionarea inapoi in fisier");
			exit(9);
		}
	}
}
