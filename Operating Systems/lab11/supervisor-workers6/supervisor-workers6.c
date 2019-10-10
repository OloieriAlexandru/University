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
#define nmax 105

int main(int argc, char *argv[])
{	
	pid_t pid;
	int n, i, p[nmax][2], nr, codr;
	if (argc == 1)
	{
		printf("Introduceti numarul de procese: \n");
		if (scanf("%d",&n) != 1 || n < 1)
		{
			fprintf(stderr, "Ai introdus o valoare invalida pentru numarul de procese\n");
			exit(1);
		}
	}
	else
	{
		if (sscanf(argv[1],"%d",&n) != 1 || n < 1)
		{
			fprintf(stderr, "Ai introdus o valoare invalida pentru numarul de procese\n");
			exit(2);
		}
	}
	for (i=0;i<n;++i)
	{
		if (-1 == pipe(p[i]))
		{
			fprintf(stderr, "Eroare la al %d-lea pipe()\n", i);
			perror("Motivul ");
			exit(3);
		}
	}

	for (i=0;i<n;++i)
	{
		if (-1 == (pid = fork()))
		{
			fprintf(stderr, "Eroare la al %d-lea fork()\n", i);
			perror("Motivul ");
			exit(4);
		}
		if (pid == 0)
		{
			dup2(p[i][0],0);
			dup2(p[i][1],1);
			execl("slave","slave",NULL);

			fprintf(stderr, "Supervisor: eroare la al %d-lea execl()\n", i);
			perror("Motivul ");
			exit(5);
		}
	}

	int k = 0;
	while (1)
	{
		codr = scanf("%d",&nr);
		if (codr == -1)
		{
			perror("Eroare la citirea din stdin");
			exit(6);
		}
		if (!nr) break;
		write(p[k][1],&nr,sizeof(nr));
		++k;
		k%=n;
	}
	int z = 0;
	for (i=0;i<n;++i)
		write(p[i][1],&z,sizeof(z));
	for (i=0;i<n;++i)
		wait(NULL);
	int ans = 0;
	for (i=0;i<n;++i)
	{
		read(p[i][0],&nr,sizeof(nr));
		close(p[i][0]);
		ans+=nr;
	}
	printf("Suma secventei introduse este: %d\n", ans);
	return 0;
}
