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
	int nr, codRead, fd;
	if (-1 == (fd = open("fis.dat", O_RDONLY)))
	{
		perror("Eroare la deschiederea fisierului de date");
		exit(1);
	}
	printf("Secventa de numere din fisierul de date este:\n");
	while(1)
	{
		if (-1 == (codRead = read(fd, &nr, sizeof(int))))
		{
			perror("Eroare la citirea din fisierului de date");
			exit(2);
		}
		if (0 == codRead)
		{
			break;
		}
		printf("%d ",nr);
	}
	printf("\n\n");
	close(fd);
	return 0;
}
