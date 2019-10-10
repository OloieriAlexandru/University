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
	int i, n, nr, fd;

	if (-1 == (fd = creat("fis.dat",0600)))
	{
		perror("Eroare la crearea fisierului de date");
		exit(1);
	}

	printf("Dati lungimea secventei de numere: ");
	scanf("%d",&n);

	for (i=1;i<=n;++i)
	{
		printf("Dati al %d-lea numar: ",i);
		scanf("%d",&nr);
		if (-1 == write(fd, &nr, sizeof(int)))
		{
			perror("Eroare la scrierea in fisierul de date");
			exit(2);
		}
	}
	close(fd);
	return 0;
}
