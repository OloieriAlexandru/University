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
	int cod, nr;
	FILE *fin, *fout;
	pid_t pid;
	if (-1 == mkfifo("pipe2", 0600))
	{
		if (errno == EEXIST)
			fprintf(stdout,"Canalul exista deja!\n");
		else
		{
			perror("Eroare la crearea canalului fifo. Cauza erorii");
			exit(1);
		}
	}

	if (-1 == (pid = fork()))
	{
		perror("Eroare la fork()\n");
		exit(2);
	}
	if (pid)
	{
		fout = fopen("pipe2", "w");

		printf("Introduceti numere intregi si terminati cu Ctrl + D\n");

		while ( (cod = scanf("%d",&nr)) != EOF)
		{
			if (cod == 1)
			{
				fprintf(fout,"%d\n",nr);
				fflush(fout);
			}
			else
			{
				fprintf(stderr, "Eroare: nu ati introduc caractere permise sa apara intr-un numar!\n");
				exit(3);
			}
		}
		fclose(fout);
		wait(NULL);
	}
	else
	{
		fin = fopen("pipe2","r");

		while (fscanf(fin,"%d",&nr) != EOF)
		{
			printf("[Fiu] Am primit numarul: %d\n", nr);
			fflush(stdout);
		}
		fclose(fin);
	}
	return 0;
}
