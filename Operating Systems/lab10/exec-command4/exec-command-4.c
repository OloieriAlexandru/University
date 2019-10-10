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
	char nume_dir[100];
	int cod_term;

	char *prm[5];
	prm[0] = "rm";
	prm[1] = "-r";
	prm[2] = "-i";
	prm[3] = nume_dir;
	prm[4] = NULL;

	if (argc > 1)
	{
		strcpy(nume_dir, argv[1]);
	}
	else
	{
		printf("Introduceti numele directorului dorit: ");
		scanf("%s",nume_dir);
		printf("\n");
	}

	switch(fork())
	{
		case -1: perror("Eroare la primul fork()"); exit(1);
		case 0: execlp("ls","ls","-l",nume_dir,NULL);
			perror("Eroare la primul exec");
			exit(2);
		default: wait(&cod_term);
			 if (WIFEXITED(cod_term))
				 printf("\nComanda ls a rulat, terminandu-se cu codul de terminare: %d\n", WEXITSTATUS(cod_term));
			 else
				 printf("\nComanda ls a fost terminata fortat de catre semnalul: %d\n", WTERMSIG(cod_term));
	}

	int i;
	printf("\nComanda care este pe cale sa fie executata: ");
	for (i=0;i<4;++i)
		printf("%s ",prm[i]);
	printf("\n\n");

	switch(fork())
	{
		case -1: perror("Eroare la al doilea fork()"); exit(1);
		case 0: execv("/bin/rm",prm);
			perror("Eroare la al doilea exec");
			exit(3);
		default: wait(&cod_term);
			 if (WIFEXITED(cod_term))
				 printf("\nComanda rm a rulat, terminandu-se cu codul de terminare: %d\n", WEXITSTATUS(cod_term));
			 else
				 printf("\nComanda rm a fost terminata fortat de catre semnalul: %d\n", WTERMSIG(cod_term));
	}

	printf("Done!\n");

	return 0;
}
