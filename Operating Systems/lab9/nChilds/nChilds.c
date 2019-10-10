#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{	
	int n, i;
	pid_t pid;
	if (argc == 1)
	{
		printf("Introduceti numarul de fii: ");
		while ( (1 != scanf("%d",&n)) && (n<1))
		{
			fprintf(stderr, "\nEroare:nu ati introdus un numar natural strict pozitiv!\n");
		}
	}
	else
	{
		if (1 != sscanf(argv[1], "%d", &n))
		{
			fprintf(stderr, "Eroare: nu ati specificat un numar natural strict pozitiv!\n");
		}
	}
	
	printf("Sunt procesul initial cu PID-ul: %d, iar parintele meu este procesul cu PID-ul: %d\n", getpid(), getppid());

	for (i=1;i<=n;++i)
	{
		if (-1 == (pid=fork()))
		{
			perror("Eroare la fork");
			exit(2);
		}
		if (0 == pid)
		{
			printf("Sunt procesul fiu %d, avand PID-ul: %d, iar parintele meu este procesul cu PID-ul: %d.\n", i, getpid(), getppid());
			return i;
		}
	}
	for (i=1;i<=n;++i)
		wait(NULL);
	return 0;
}
