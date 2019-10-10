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

int main(int argc, char *argv[])
{	
	pid_t pid;
	int codRet;
	char input[11];
       	int sz = sizeof(input);
	char num1[11], num2[11], opr[2];
	int running = 1;
	int nr1, nr2;
	char op;
	while (running)
	{
		while (1)
		{
			memset(input, 0, sizeof(input));
			getLine("nr1>",input,sz);
			if (!strcmp(input,"close"))
			{
				running = 0;
				break;
			}
			if (1 != sscanf(input,"%d",&nr1))
			{
				fprintf(stderr, "Trebuie sa introduci un numar natural intreg!\n");
				continue;
			}
			break;
		}
		if (!running) continue;
		while (1)
		{
			memset(input, 0, sizeof(input));
			getLine("op>",input,sz);
			if (!strcmp(input,"close"))
			{
				running = 0;
				break;
			}
			if (1 != sscanf(input,"%c",&op) || (op != '+' && op != '-'))
			{
				fprintf(stderr, "Trebuie sa introduci unul dintre urmatoarele caractere: +/-\n");
				continue;
			}
			break;
		}
		if (!running) continue;
		while (1)
		{
			memset(input, 0, sizeof(input));
			getLine("nr2>",input,sz);
			if (!strcmp(input, "close"))
			{
				running = 0;
				break;
			}
			if (1 != sscanf(input,"%d",&nr2))
			{
				fprintf(stderr, "Trebuie sa introduci un numar natural intreg!\n");
				continue;
			}
			break;
		}
		if (!running) continue;
		
		memset(num1, 0, 11);
		memset(num2, 0, 11);
		memset(opr, 0, 2);

		sprintf(num1,"%d",nr1);
		sprintf(num2,"%d",nr2);
		sprintf(opr,"%c",op);

		if (-1 == (pid=fork()))
		{
			perror("Eroare la fork");
			exit(1);
		}

		if (pid == 0)
		{
			execl("slave","slave",num1,opr,num2,NULL);
		
			fprintf(stderr, "Eroare la apelul execl() dintr-un proces slave!\n");
			perror("Cauza erorii: ");
			exit(255);
		}

		wait(&codRet);
		if ((codRet >> 8) == 255)
		{
			printf("Procesul slave a avut o eroare\n");
		}
		else
		{
			printf("%d%c%d=%d\n",nr1,op,nr2,codRet>>8);
		}	
	}
	return 0;
}
