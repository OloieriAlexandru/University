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
	char *pOut = "pipeOut";
	char pIn[nmax][nmax];
	memset(pIn,0,sizeof(pIn));
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
		sprintf(pIn[i],"pipe%din",i);
		if (-1 == mkfifo(pIn[i],0600) && errno != EEXIST)
		{
			fprintf(stderr, "Eroare la crearea celui de-al %d-lea pipe\n", i+1);
			perror("Cauza");
			exit(100);
		}
	}

	if (-1 == mkfifo(pOut,0600))
	{
		if (errno != EEXIST)
		{
			perror("Eroare la crearea pipe ului pipeOut, cauza");
			exit(152);
		}
	}

	struct flock block, unblock;
	block.l_type = F_WRLCK;
	unblock.l_type = F_UNLCK;
	block.l_whence = unblock.l_whence = SEEK_CUR;
	block.l_start = unblock.l_start = 0;
	block.l_len = unblock.l_len = 4;

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
			int codr, fd = open(pIn[i],O_RDONLY);
			int sum = 0, nr;
			int fdr = open(pOut,O_WRONLY);
			while (1)
			{
				codr = read(fd, &nr, sizeof(nr));
				if (codr == -1)
				{
					fprintf(stderr, "Eroare in citire in al %d-lea fiu\n", i+1);
					perror("Cauza");
					exit(123);
				}
				if (!codr) break;
				sum += nr;
			}
			close(fd);
			if (-1 == fcntl(fdr, F_SETLKW, &block))
			{
				fprintf(stderr, "Eroare la punerea lacatului in al %d-lea fiu", i+1);
				perror("Cauza");
				exit(199);		
			}
			write(fdr, &sum, sizeof(sum));
			if (-1 == fcntl(fdr, F_SETLKW, &unblock))
			{
				perror("Eroare la scoaterea lacatului");
				exit(201);
			}
			close(fdr);
			exit(0);		
		}
	}
	int k = 0;
	int fdiOut[nmax];
	memset(fdiOut,0,sizeof(fdiOut));
	for (i=0;i<n;++i)
		fdiOut[i] = open(pIn[i],O_WRONLY);
	int fdRs = open(pOut,O_RDONLY);
	printf("Introduceti o secvena de numere, terminata prin 0\n");
	while (1)
	{
		codr = scanf("%d",&nr);
		if (codr == -1)
		{
			perror("Eroare la citirea din stdin");
			exit(6);
		}
		if (!nr || !codr) break;
		write(fdiOut[k],&nr,sizeof(nr));
		++k;
		k%=n;
	}

	for (i=0;i<n;++i)
		close(fdiOut[i]);
	for (i=0;i<n;++i)
		wait(NULL);
	int ans = 0;
	while (1)
	{
		codr = read(fdRs, &nr, sizeof(nr));
		if (codr == -1)
		{
			perror("Eroare la citirea raspunsului unui fiu din pipe");
			exit(241);
		}
		if (!codr) break;
		ans += nr;
	}
	printf("Suma secventei introduse este: %d\n", ans);
	for (i = 0;i<n;++i)
		remove(pIn[i]);
	remove(pOut);
	return 0;
}
