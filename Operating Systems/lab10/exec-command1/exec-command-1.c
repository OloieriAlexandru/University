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
	pid_t pid;
	int cod_term;
	char *dirname;

	dirname = (argc < 2) ? "." : argv[1];

	/* creez un proces fiu, care va rula comanda ls */
	if (-1 == (pid=fork()))
	{
		perror("Eroare la fork");
		exit(1);
	}

	if (pid == 0)
	{
		execlp("ls","ls","-i","-l",dirname,NULL);

		perror("Eroare la exec");
		exit(10);
	}
	wait(&cod_term);
	if (cod_term >> 8 == 10)
	    printf("Comanda ls nu a putut fi executata... (apelul exec a esuat).\n");
	else
	{
		printf("Comanda ls a fost executata...");
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
