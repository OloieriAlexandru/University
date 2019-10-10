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

#define READ_OK 0
#define READ_NO_INPUT 1
#define READ_TOO_LONG 2

int getLine(char *prompt, char *buff, int sz)
{
	char ch;
	int extra;
	if (prompt)
	{
		printf("%s",prompt);
		fflush(stdout);
	}
	if (fgets(buff, sz, stdin) == NULL)
		return READ_NO_INPUT;
	if (buff[strlen(buff)-1] != '\n') {
		extra = 0;
		while (((ch = getchar()) != '\n') && (ch != EOF))
			extra = 1;
		return (extra == 1) ? READ_TOO_LONG : READ_OK;
	}
	buff[strlen(buff)-1] = '\0';
	return READ_OK;
}
	
int main()
{
	pid_t pid;
	int p[2];

	if(-1 == pipe(p) )
	{
		fprintf(stderr, "Eroare: nu pot crea un canal anonim, errno=%d\n",errno);  perror("Cauza erorii");  exit(1);
	}

	if(-1 == (pid=fork()) )
	{
		perror("Eroare: nu am putut crea un fiu. Cauza erorii");  exit(2);
	}

	if(pid == 0)
	{
		if(-1 == dup2(p[1],1) )
		{
			perror("Eroare: redirectarea iesirii standard in fiu a esuat. Cauza erorii");  exit(3);
		}

		execlp("grep","grep","/bin/bash","/etc/passwd",NULL);
		perror("Eroare la execlp() in fiu. Cauza erorii");  exit(4);
	}
	else
	{
		close(p[1]); // necesar pentru ca sa se termine executia comenzii cut (prin citirea EOF din canal)

		if(-1 == dup2(p[0],0) )
		{
			perror("Eroare: redirectarea intrarii standard in parinte a esuat. Cauza erorii");  exit(5);
		}

		execlp("cut","cut","-d,","-f1",NULL);
		perror("Eroare la execlp() in parinte. Cauza erorii");  exit(6);
	}

	return 0;  // fluxul de execuție nu va ajunge niciodată aici!
}
