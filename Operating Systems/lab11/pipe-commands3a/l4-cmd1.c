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
	int p[2];
	if (-1 == pipe(p))
	{
		fprintf(stderr, "Eroare: nu pot crea un canal anonim, errno=%d\n",errno);
		perror("Cauza erorii");
		exit(1);
	}
	if(-1 == (pid = fork()))
	{
		perror("Eroare: nu am putut crea un fiu. Cauza erorii");
		exit(2);
	}

	if (pid == 0)
	{
		if (-1 == dup2(p[1], 1))
		{
			perror("Eroare: redirectarea iesirii standard in fiu a esuat. Cauza erorii");
			exit(3);
		}
		execlp("grep","grep","/bin/bash","/etc/passwd",NULL);
		perror("Eroare la primul apel execlp() in fiu. Cauza erorii");
		exit(4);
	}
	else
	{
		close(p[1]);
		if (-1 == dup2(p[0], 0))
		{
			perror("Eroare: redirectarea intrarii stanrd in parinte a esuat. Cauza erorii");
			exit(5);
		}
		execl("exe2","cmd4-1",NULL);
		perror("Eroare la primul apel execl(). Cauza erorii");
		exit(6);
	}
	return 0;
}
