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
#define BUFFSZ 222

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

int main(int argc, char *argv[])
{	
	pid_t pid;
	int running = 1, act, returnCode;
	char buff[BUFFSZ];
	char *par[100], *point;
	while (running)
	{
		memset(buff, 0, BUFFSZ);
		getLine("MyShell>", buff, BUFFSZ);
		if (!strcmp(buff, "exit"))
		{
			running = 0;
			continue;
		}
		act = 0;
		point = strtok(buff, " ");
		while (point)
		{
			par[act++] = point;
			point = strtok(NULL, " ");
		}
		par[act] = NULL;
		if (-1 == (pid = fork()))
		{
			perror("Eroare la fork");
			return 1;
		}
		if (pid == 0)
		{
			execvp(par[0], par);

			return 129;
		}
		wait(&returnCode);
		if ((returnCode >> 8) == 129)
		{
			printf("Comanda ta avea a generat o eroare!\n");
		}
	}
	return 0;
}
