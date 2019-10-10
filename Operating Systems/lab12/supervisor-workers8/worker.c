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

#define nmax 10000
#define lmax 100
char cmd[nmax];
int ln;

int getNextCmd(int fd)
{
	memset(cmd,0,sizeof(cmd));
	char flag, ch;
	int codr;
	if (-1 == read(fd, &flag, 1))
	{
		perror("Eroare la citirea flagului din pipe");
		exit(1);
	}
	if (flag == 0)
	{
		// flag 0 = nu mai am comenzi
		// flag 1 = urmeaza o comanda, ce se incheie cu caracterul 1
		return 0;
	}
	ln = 0;
	while (1)
	{
		codr = read(fd, &ch, 1);
		if (codr == -1)
		{
			perror("Eroare la citirea caracterelor din pipe");
			exit(2);
		}
		if (ch == 1)
			break;
		cmd[ln++] = ch;
	}
	if (ln == 0) return 0;
	return 1;
}

void work(int fd, int fdw)
{
	pid_t pid;
	int doo, ret, cnt, cod;
	char *fv[lmax], *p;
	while (1)
	{
		doo = getNextCmd(fd);
		if (doo == 0)
		{
			// am terminat toate comenzile, ma opresc
			break;
		}
		// am mai primit o comanda, trebuie sa fac fork
		// execut comanda
		// apoi trimit inapoi codul de terminare prin pipe
		memset(fv, 0, sizeof(fv));
		cnt = 0;
		p = strtok(cmd, " ");
		while (p)
		{
			fv[cnt++] = p;
			p = strtok(NULL, " ");
		}
		fv[cnt] = NULL;
		
		if (-1 == (pid = fork()))
		{
			perror("Eroare la fork()");
			exit(125);
		}
		if (pid == 0)
		{
			execvp(fv[0], fv);

			exit(130);
		}
		wait(&cod);
		ret = (cod >> 8);
		write(fdw,&ret,sizeof(ret));
	}
}

int main(int argc, char *argv[])
{	
	int fd, fdo;
	char *pipeName = "socket";
	char *retRez = "sendRet";
	if (-1 == mkfifo(pipeName, 0600) && errno != EEXIST)
	{
		perror("Eroare la crearea pipe ului socket");
		exit(1);
	}
	if (-1 == mkfifo(retRez, 0600) && errno != EEXIST)
	{
		perror("Eroare la crearea pipe ului sendRez");
	}
	fd = open(pipeName, O_RDONLY);
	fdo = open(retRez, O_WRONLY);
	if (fd == -1)
	{
		perror("Eroare la deschiderea pipe ului in supervisor");
		exit(2);
	}
	
	work(fd, fdo);
	close(fd);
	close(fdo);

	return 0;
}
