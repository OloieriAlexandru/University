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
#define BUFFSZ 555
#define nmax 255
#define nmax2 15

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

int nrc(int x)
{
	if (!x) return 1;
	int ans = 0;
	while (x) ++ans, x/=10;
	return ans;
}

void setNums(char *s, int nr1, int nr2)
{
	memset(s, 0, sizeof(s));
	int ln1 = nrc(nr1), ln2 = nrc(nr2);
	int ln = ln1 + ln2 + 1, i;
	for (i = ln1-1;i>=0;--i)
		s[i] = nr1 % 10 + '0', nr1/=10;
	s[ln1] = '-';
	for (i = ln-1;i>ln1;--i)
		s[i] = nr2 % 10 + '0', nr2/=10;
}

void compressSpacesString(char *s)
{
	int ln = strlen(s);
	int i, j;
	for (i=0;i<ln;++i)
	{
		while (i<ln-1 && s[i] == s[i+1] && s[i] == ' ')
		{
			for (j=i+1;j<ln;++j)
				s[j] = s[j+1];
			--ln;
		}
	}
	while (ln > 0 && s[ln-1] == '|') --ln;
}

void changeSpaces(char *s)
{
	int i, ln = strlen(s);
	if (!ln) return;
	if (s[0] == ' ') s[0] = 1;
	if (s[ln-1] == ' ') s[ln-1] = 1;
	for (i = 1; i<ln-1;++i)
		if (s[i] == ' ' && ( (s[i-1] == '\"' && s[i+1] == '\"') || (s[i-1] == '\'' && s[i+1] == '\'')  ))
			s[i] = 1, s[i+1] = 1, s[i-1] = ' ';
		else if (s[i] == ' ')
			s[i] = 1;
}

int main(int argc, char *argv[])
{	
	pid_t pid;
	int running = 1, act, returnCode;
	char buff[BUFFSZ];
	char nums[nmax2];
	char *par[nmax], *point;
	int nr1 = 1, nr2, ln, i;
	while (running)
	{
		memset(buff, 0, BUFFSZ);
		getLine("MyShell>", buff, BUFFSZ);
		if (!strcmp(buff, "exit"))
		{
			running = 0;
			continue;
		}
		compressSpacesString(buff);
		nr2 = 0; act = 2;
		point = strtok(buff, "|");
		while (point)
		{
			changeSpaces(point);
			par[act++] = point;
			++nr2;
			point = strtok(NULL, "|");
		}
		if (!nr2) nr2 = 1;
		memset(nums, 0, sizeof(nums));
		setNums(nums, nr1, nr2);
		par[0] = "pip";
		par[1] = nums;
		par[act] = NULL;
		if (-1 == (pid = fork()))
		{
			perror("Eroare la fork");
			return 1;
		}
		if (pid == 0)
		{
			execv("pip", par);
			perror("Nu am putut rula fisierul recursiv");
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
