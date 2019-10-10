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
		perror("Nu s-a putut crea un canal anonim");
		exit(1);
	}
	if (-1 == (pid = fork()))
	{
		perror("Eroare la fork in primul fisier din lant");
		exit(3);
	}

	if (pid == 0)
	{
		if (-1 == dup2(p[1],1))
		{
			perror("Eroare la redirectarea iesirii standard");
			exit(4);
		}
		execlp("w","w","-h",NULL);
		perror("Eroare la execlp()");
		exit(5);
	}
	else
	{
		close(p[1]);
		if (-1 == dup2(p[0],0))
		{
			perror("Eroare la redirectarea intrarii standard");
			exit(5);
		}
		execl("exe2","p3-cmd2",NULL);
		perror("Eroare la execl()");
		exit(6);
	}
	return 0;
}
