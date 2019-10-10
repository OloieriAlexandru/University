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

#define nmax 101

struct info
{
	int nr1, nr2;
};
/*
 * Se apeleaza recursiv: 
 * argv[2] contine un string de forma: nr1-nr2
 * nr1 = numarul apelului curent
 * nr2 = numarul total de apeluri recursive
 * argv[3] ... argv[2+nr2]
 */
int nrc(int x)
{
	if (!x) return 1;
	int ans = 0;
	while (x) ++ans, x/=10;
	return ans;
}
struct info getNr1nr2(char *s)
{
	char *p;
	int nr1 = -1, nr2 = -1;
	p = strtok(s,"-");
	while (p)
	{
		if (nr1 == -1)
			nr1 = atoi(p);
		else
			nr2 = atoi(p);
		p = strtok(NULL,"-");
	}
	struct info ans;
	ans.nr1 = nr1;
	ans.nr2 = nr2;
	return ans;
}
void buildArgv1(char **s, int nr1, int nr2)
{
	int needed = 1, i;
	int ln1 = nrc(nr1);
	int ln2 = nrc(nr2);
	needed += ln1 + ln2;
	*s = (char*) (malloc((size_t)needed));
	memset(*s,0,sizeof(*s));
	for (i = ln1 - 1;i>=0;--i)
	    (*s)[i] = nr1 % 10 + '0', nr1/=10;
	(*s)[ln1] = '-';
	for (i = needed - 1;i>ln1;--i)
	    (*s)[i] = nr2 % 10 + '0', nr2/=10;
}
int main(int argc, char *argv[])
{	
	pid_t pid;
	char *par[nmax], *point;
	int p[2], now, i;
	if (!argv[1])
	{
		fprintf(stderr, "Programul a fost apelat intr-un mod incorect... pare rau\n");
		exit(127);
	}
	struct info act = getNr1nr2(argv[1]);
	if (act.nr1 == act.nr2)
	{
		// mai am o singura comanda de apelat
		// si anume comanda argv[1+act.nr1]
		now = 0;
		point = strtok(argv[1+act.nr1], "\1");
		while (point)
		{
			par[now++] = point;
			point = strtok(NULL, "\1");
		}
		par[now] = 0;
		execvp(par[0], par);
		fprintf(stderr, "Eroare in apelul %s la execlp()\n", argv[1]);
	exit(125);
	}
	else
	if (act.nr1 + 1 == act.nr2)
	{
		// mai am doua comenzi de apelat
		if (-1 == pipe(p))
		{
			perror("Eroare la crearea unui canal anonim");
			exit(125);
		}
		if (-1 == (pid = fork()))
		{
			perror("Eroare la fork()");
			exit(125);
		}
		if (pid == 0)
		{
			if (-1 == dup2(p[1],1))
			{
				perror("Eroare la redirectarea iesirii standard");
				exit(125);
			}
			now = 0;
			point = strtok(argv[1+act.nr1],"\1");
			while (point)
			{
				par[now++] = point;
				point = strtok(NULL, "\1");
			}
			par[now] = 0;
			execvp(par[0], par);
			fprintf(stderr, "Eroare in apelul copil %s la execlp()\n", argv[1]);
		}
		else
		{
			close(p[1]);
			if (-1 == dup2(p[0],0))
			{
				perror("Eroare la redirectarea intrarii standard");
				exit(125);
			}
			now = 0;
			point = strtok(argv[2+act.nr1],"\1");
			while (point)
			{
				par[now++] = point;
				point = strtok(NULL, "\1");
			}
			par[now] = 0;
			execvp(par[0], par);
			fprintf(stderr, "Eroare in apelul copil %s la execlp()\n", argv[1]);
		}
	}
	else
	{
		char *s;
		buildArgv1(&s,act.nr1+1,act.nr2);
		// mai am mai multe comenzi
		if (-1 == pipe(p))
		{
			perror("Eroare la crearea unui canal anonim");
			exit(125);
		}
		if (-1 == (pid = fork()))
		{
			perror("Eroare la fork");
			exit(125);
		}
		if (pid == 0)
		{
			if (-1 == dup2(p[1],1))
			{
				perror("Eroare la redirectarea iesirii standard");
				exit(125);
			}
			now = 0;
			point = strtok(argv[1+act.nr1],"\1");
			while (point)
			{
				par[now++] = point;
				point = strtok(NULL, "\1");
			}
			par[now] = 0;
			execvp(par[0], par);
			fprintf(stderr, "Eroare in apelul copil %s la execlp()\n", argv[1]);
		}
		else
		{
			close(p[1]);
			if (-1 == dup2(p[0],0))
			{
				perror("Eroare la redirectarea intrarii standard");
				exit(125);
			}
			argv[1] = s;
			execvp("./pip", argv);
			fprintf(stderr, "Eroare la apelul %s al scriptului recursiv\n", s);
			perror("Cauza erorii");
			exit(125);
		}
	}
	return 0;
}
