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

char fileName[] = "instr";

int main(int argc, char *argv[])
{	
	int n, i, fd;
	int cheie, valoare;
	int codRead;
	if (argc == 1)
	{
		printf("Trebuie sa specifici indicele fisierului din care citesc\n");
		return 0;
	}
	int nr;
	if (1 != sscanf(argv[1], "%d",&nr))
	{
		fprintf(stderr, "[PID: %d] Eroare: ati specificat o valoarea invalida drept argument", getpid());
		exit(1);
	}
	int ln = 0, cnr = nr;
	while (cnr) ++ln, cnr/=10;
	int newLen = strlen(fileName) + ln;
	if (nr == -1)
		newLen = strlen("fis.dat") + 1;
	char *newFileName = (char*) malloc(newLen);
	memset(newFileName, 0, sizeof(newFileName));
	if (nr != -1)
	{
		strcpy(newFileName, fileName);
		for (i=newLen-1;nr;--i)
			newFileName[i] = (nr%10 + '0'), nr/=10;	
	}
	else
	{
		strcpy(newFileName, "fis.dat");
	}
	if (-1 == (fd = open(newFileName, O_RDONLY)))
	{
		printf("Eroare la deschiderea fisierului %s\n", newFileName);
		perror("Motivul");
		free(newFileName);
		return 1;
	}

	while (0 != (codRead = read(fd, &cheie, sizeof(int))))
	{
		if (codRead == -1)
		{
			perror("Eroare la citirea cheii dintr-o pereche");
			free(newFileName);
			close(fd);
			exit(3);
		}
		codRead = read(fd, &valoare, sizeof(int));
		if (!codRead) break;
		if (codRead == -1)
		{
			perror("Eroare la citirea valorii dintr-o pereche");
			free(newFileName);
			close(fd);
			exit(4);
		}
		printf("(%d,%d)\n", cheie, valoare);
	}

	free(newFileName);
	close(fd);
	return 0;
}
