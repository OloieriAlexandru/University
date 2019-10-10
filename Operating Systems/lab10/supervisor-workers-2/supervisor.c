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

#define MAX_NO_WORKERS 10
#define MAX_LENGTH_FILENAME 50

int main(int argc, char *argv[])
{	
	int nr = 0, k, NW = 0, i, pid, sum=0;
	char input_filenames[MAX_NO_WORKERS][MAX_LENGTH_FILENAME];
	char output_filenames[MAX_NO_WORKERS][MAX_LENGTH_FILENAME];
	FILE *fdi[MAX_NO_WORKERS], *fdo;

	printf("Dati numarul de procese workers: ");
	while (1 != scanf("%d",&NW))
	{
		fprintf(stderr, "Trebuie sa introduceti un numar intreg pozitiv! Incercati din nou...\n");
	}
	if ( (NW < 1) || (NW > MAX_NO_WORKERS))
	{
		fprintf(stderr, "Date de intrare incorecte.\n");
		exit(1);
	}

	for (i=1;i<=NW;++i)
	{
		sprintf(input_filenames[i], "f%di.txt", i);
		sprintf(output_filenames[i],"f%do.txt", i);
	}

	for (i=1;i<=NW;++i)
		if (NULL == (fdi[i-1] = fopen(input_filenames[i],"w")))
		{
			fprintf(stderr, "Supervisor: eroare la deschiderea fisierului de intrare %s.\n", input_filenames[i]);
			perror("Cauza erorii: ");
			exit(2);
		}

	printf("Introduceti secventa de numere (0 pentru terminare): \n");
	k = 0;
	do
	{
		while (1 != scanf("%d",&nr))
		{
			fprintf(stderr, "Trebuie sa introduceti un numar intreg nenul! Incercati din nou...");
		}
		if (nr != 0)
			fprintf(fdi[k], "%d ", nr);
		++k;
		k %= NW;
	} while (nr != 0);

	for (i = 1;i<=NW;++i)
		fclose(fdi[i-1]);

	for (i = 1;i<=NW;++i)
	{
		if (-1 == (pid = fork()))
		{
			fprintf(stderr, "Supervisor: eroare la apelul fork() pentru procesul worker al %d-lea.\n", i);
			perror("Cauza erorii: ");
			exit(3);
		}

		if (pid == 0)
		{
			execl("worker", "worker", input_filenames[i], output_filenames[i], NULL);
			fprintf(stderr, "Supervisor: eroare la apelul exec() pentru procesul worker al %d-lea.\n",i);
			perror("Cauza erorii: ");
			exit(4);
		}
	}

	for (i=1;i<=NW;++i)
		wait(NULL);

	for (i = 1;i<=NW;++i)
	{
		if (NULL ==  (fdo = fopen(output_filenames[i], "r")))
		{
			fprintf(stderr,"Supervisor: eroare la deschiderea fisierului de iesire: %s.\n", output_filenames[i]);
			perror("Cauza erorii: ");
			exit(5);
		}
		if (1 != fscanf(fdo, "%d",&nr))
		{
			fprintf(stderr, "Supervisor: eroare la citirea celei de a %d-a sume partiale.\n", i);
			exit(6);
		}
		sum+=nr;
		fclose(fdo);
	}

	printf("Supervisor: suma secventei introduse este: %d.\n", sum);
	for (i=1;i<=NW;++i)
	{
		remove(input_filenames[i]);
		remove(output_filenames[i]);
	}

	return 0;
}
