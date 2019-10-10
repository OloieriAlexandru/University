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

int n, k;

void rec(int nivel, int j)
{
	if (nivel == n) return;
	int i;
	pid_t pid;
	for (i = 1; i<=k; ++i)
	{
		if (-1 == (pid = fork()))
		{
			perror("Eroare la fork");
			exit(3);
		}
		if (pid == 0)
		{
			// sunt in fiu, trebuie sa intru intr-o recursie
			rec(nivel+1, i);
			exit(3);
		}
	}
	printf("%d-%d\n",getppid(), getpid());
	int returnCode;
	printf("%d,Codurile return al copiilor mei: ", getpid());
	for (i=1;i<=k;++i)
	{
	
		wait(&returnCode);
		printf("%d ",returnCode>>8);
	}
	printf("\n");
	exit(3);
}

int main(int argc, char *argv[])
{	
	if (argc >= 3)
	{
		if (1 != sscanf(argv[1], "%d", &n) || n <= 0)
		{
			fprintf(stderr, "N trebuie sa fie un numar natural pozitiv!\n");
			return 1;
		}
		if (1 != sscanf(argv[2], "%d", &k) || k <= 0)
		{
			fprintf(stderr, "K trebuie sa fie un numar natural pozitiv!\n");
			return 2;
		}
	}
	else
	if (argc == 2)
	{
		if (1 != sscanf(argv[1], "%d", &n) || n <= 0)
		{
			fprintf(stderr, "N trebuie sa fie un numar natural pozitiv!\n");
			return 1;
		}
		printf("k=");
		if (1 != scanf("%d",&k) || k <= 0)
		{
			fprintf(stderr, "K trebuie sa fie un numar natural pozitiv!\n");
			return 2;
		}
	}
	else
	{
		printf("n=");
		if (1 != scanf("%d",&n) || n <= 0)
		{
			fprintf(stderr, "N trebuie sa fie un numar natural pozitiv!\n");
		       	return 1;
		}
		printf("k=");
		if (1 != scanf("%d",&k) || k <= 0)
		{
			fprintf(stderr, "K trebuie sa fie un numar natural pozitiv!\n");
			return 2;
		}
	}
	printf("%d\n", getpid());

	rec(1,1);
	return 0;
}
