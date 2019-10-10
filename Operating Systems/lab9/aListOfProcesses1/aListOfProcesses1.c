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
	int n, i, codTerm;
	pid_t pid;

	if (argc == 1)
	{
		printf("Introduceti numarul de procese: ");
		while ( (1 != scanf("%d",&n)) && (n<1))
		{
			fprintf(stderr, "\nEroare: nu ati introdus un numar strict pozitiv!\n");
		}
	}
	else
	{
		if (1 != sscanf(argv[1], "%d",&n))
		{
			fprintf(stderr, "\nEroare: nu ati introdus un numar strict pozitiv!\n");
		}
	}
	for (i = 1; i < n; ++i)
	{
		if (-1 == (pid = fork()))
		{
			perror("Eroare la fork");
			exit(2);
		}
		if (0 != pid)
		{
			wait(&codTerm);

			printf("Sunt procesul %d, avand PID-ul: %d, parintele are PID-ul: %d, iar fiul creat are PID-ul: %d si s-a terminat cu codul %d.\n", i, getpid(), getppid(), pid, codTerm>>8);

			return i;
		}
		else
		{
			if (i == n-1)
			{
				printf("Sunt procesul ultim, %d din lantul de procese, avand PID-ul: %d, iar parintele meu este procesul cu PID-ul: %d\n", n, getpid(), getppid());
				return n;
			}
		}
	}
	return 0;
}
