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
	int pid, nr, p[2];
	FILE *fin, *fout;

	if (-1 == pipe(p))
	{
		perror("Eroare la crearea canalului intern!");
		exit(1);
	}
	fin = fdopen(p[0], "r");
	fout = fdopen(p[1], "w");

	if (-1 == (pid = fork()))
	{
		perror("Eroare la crearea unui proces fiu!");
		exit(2);
	}
	if (pid)
	{
		fclose(fin);
		printf("Introduceti o secventa de numere diverse, terminata prin CTRL+D.\n");
		while (EOF != scanf("%d",&nr))
		{
			fprintf(fout,"%d\n",nr);
			fflush(fout);
		}
		fclose(fout);
		wait(NULL);
	}
	else
	{
		fclose(fout);
		while (fscanf(fin,"%d\n",&nr) != EOF)
		{
			printf("[Procesul consumator (fiul)] Am primit numarul: %d\n", nr);
			fflush(stdout);
		}
		printf("[Procesul consumator (fiul)] Sfarsit: am citit EOF din canal!\n");

		fclose(fin);
	}
	return 0;
}
