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
	if (argc == 1)
	{
		printf("Trebuie sa specifici indicele fisierului cu comenzi creat\n");
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

	if (-1 == (fd = open(newFileName, O_CREAT | O_TRUNC | O_WRONLY, 0600)))
	{
		printf("Eroare la crearea fisierului %s\n", newFileName);
		perror("Motivul");
		free(newFileName);
		return 1;
	}

	printf("Numarul de instructiuni din fisier: ");
	while ( (1 != scanf("%d",&n)) && (n < 0))
	{
		fprintf(stderr, "Eroare, nu ati introdus un numar natural strict pozitiv!\n");
		free(newFileName);
		close(fd);
		exit(4);
	}
	printf("\nPerechile trebuie introduse sub forma: cheie valoare\n");
	for (i=1;i<=n;++i)
	{
		scanf("%d %d",&cheie, &valoare);
		if (-1 == write(fd, &cheie, sizeof(int)))
		{
			perror("Eroare la scrierea cheii in fisierul de date");
			free(newFileName);
			close(fd);
			exit(3);
		}
		if (-1 == write(fd, &valoare, sizeof(int)))
		{
			perror("Eroare la scrierea valorii in fisierul de date");
			free(newFileName);
			close(fd);
			exit(4);
		}
	}
	free(newFileName);
	close(fd);
	return 0;
}
