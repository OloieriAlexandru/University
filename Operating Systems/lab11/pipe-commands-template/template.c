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
		perror("Eroare la crearea unui canal anonim");
		exit(1);
	}
	if (-1 == (pid = fork()))
	{
		perror("Eroare la fork()");
		exit(2);
	}
	if (pid == 0)
	{
		if (-1 == dup2(p[1],1))
		{
			perror("Eroare la redirectarea iesirii standard");
			exit(3);
		}
		
		execlp("cat","cat","/etc/passwd",NULL);
		perror("Eroare la execlp()");
		exit(4);
	}
	else
	{
		close(p[1]);

		if (-1 == dup2(p[0],0))
		{
			perror("Eroare la redirectarea intrarii standard");
			exit(5);
		}		
		execlp("tr","tr","-s",":","@",NULL);
		perror("Eroare la execlp()");
		exit(6);
	}
	return 0;
}
