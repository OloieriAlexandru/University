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

void bubble_sort_c_b(int fd);

int main(int argc, char *argv[])
{	
	int fd;
	if (argc < 2)
	{
		printf("Usage: %s datafile\n", argv[0]); exit(1);
	}
	if (-1 == (fd = open(argv[1], O_RDWR)))
	{
		perror("Eroare la deschiderea fisierului de date");
		exit(2);
	}
	bubble_sort_c_b(fd);
	close(fd);
	printf("Notificare: [PID: %d] Am terminat (contributia mea) la sortarea fisierului %s !\n", getpid(), argv[1]);
	return 0;
}

void bubble_sort_c_b(int fd)
{
	int rcod1, rcod2;
	int numar1, numar2;
	int numar1recitit, numar2recitit;

	struct flock block;
	struct flock unblock;

	block.l_type = F_WRLCK;
	block.l_whence = SEEK_CUR;
	block.l_start = 0;
	block.l_len = 2*sizeof(int);

	unblock.l_type = F_UNLCK;
	unblock.l_whence = SEEK_CUR;
	unblock.l_start = 0;
	unblock.l_len = 2*sizeof(int);
	
	int modificare = 1;
		
	while(modificare)
	{
		modificare = 0;

		while(1)
		{
			rcod1 = read(fd, &numar1, sizeof(int));
			if (rcod1 == 0) break;
			if (rcod1 == -1)
			{
				perror("Eroare la citirea primului numar dintr-o pereche");
				exit(3);
			}

			rcod2 = read(fd, &numar2, sizeof(int));
			if (rcod2 == 0) break;
			if (rcod2 == -1)
			{
				perror("Eroare la citirea celui de-al doilea numar dintr-o pereche");
				exit(4);
			}

			if (numar1 > numar2)
			{
				if (-1 == lseek(fd, -2*sizeof(int), SEEK_CUR))
				{
					perror("Eroare (1) la repozitionarea inapoi in fisier");
					exit(5);
				}
				if (-1 == fcntl(fd, F_SETLKW, &block))
				{
					perror("Eroare la blocaj");
					exit(6);
				}
				if (-1 == read(fd, &numar1recitit, sizeof(int)))
				{
					perror("Eroare la recitirea primului numar dintr-o pereche");					     exit(7);
				}
				if (-1 == read(fd, &numar2recitit, sizeof(int)))
				{
					perror("Eroare la recitirea celui de-al doilea numar dintr-o pereche");
					exit(8);
				}
				if (numar1recitit > numar2recitit)
				{
					modificare = 1;
					if (-1 == lseek(fd, -2*sizeof(int), SEEK_CUR))
					{
						perror("Eroare (4) la repozitionarea inapoi in fisier"); 
						exit(9);
					}
					if (-1 == write(fd, &numar2recitit, sizeof(int)))
					{
						perror("Eroare la rescrierea primului numar dintr-o pereche");
						exit(10);
					}
					if (-1 == write(fd, &numar1recitit, sizeof(int)))
					{
						perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche");
						exit(11);
					}
				}

				if (-1 == fcntl(fd, F_SETLKW, &unblock))
				{
					perror("Eroare la deblocaj");
					exit(12);
				}
			}
			if (-1 == lseek(fd, -sizeof(int), SEEK_CUR))
			{
				perror("Eroare (2) la repozitionarea inapoi in fisier");
				exit(13);
			}
		}
		if (-1 == lseek(fd, 0L, SEEK_SET))
		{
			perror("Eroare (3) la repozitionarea inapoi in fisier");
			exit(14);
		}
	}
}
