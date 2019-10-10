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
	if (argc == 1)
	{
		fprintf(stderr, "Programul trebuie apelat cu un parametru (un nume de utilizator).\n");
		exit(1);
	}
	pid_t pid;
	int cod_term;

	if (-1 == (pid = fork()))
	{
		perror("Eroare la fork");
		exit(2);
	}
	if (pid == 0)
	{
		execlp("finger", "finger", "-p", "-m", argv[1], NULL);

		perror("Eroare la exec");
		exit(10);
	}

	wait(&cod_term);
	if(cod_term >> 8 == 10)
	{
		printf("Comanda finger nu a putut fi executata... (apelul exec a esuat).\n");
	}
	else
	{
		printf("Comanda finger a fost executata...");
		if (cod_term == 0)
			printf(" cu succes!\n");
		else
			if (WIFEXITED(cod_term))
				printf(", dar a esuat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(cod_term));
			else
				printf(", dar a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(cod_term));
	}
	return 0;
}
