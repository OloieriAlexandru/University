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
	pid_t pid_fiu;
	if (-1 == (pid_fiu = fork()))
	{
		perror("Eroare la fork");
		return 1;
	}
	if (pid_fiu == 0)
	{
		printf("\nProcesul fiu, cu PID-ul: %d.\n", getpid());
		return 3;
	}
	else
	{
		int cod_term;
		pid_fiu = wait(&cod_term);
		printf("\nProcesul tata: fiul cu PID-ul %d s-a sfarsit cu status-ul: %d.\n", pid_fiu, cod_term);

		if (WIFEXITED(cod_term))
			printf("Mai exact, codul de terminare al fiului este de fapt: %d.\n", WEXITSTATUS(cod_term));
		return 0;
	}
}
