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
char cmd[nmax], *filein = "comenzi.sh";
int ln;

int getNextCmd(int fdi)
{
	memset(cmd,0,sizeof(cmd));
	char ch;
	int codr;
	ln = 0;
	while (1)
	{
		codr = read(fdi, &ch, 1);
		if (codr == -1)
		{
			perror("Eroare la citirea din fisierul comenzi.sh");
			exit(57);
		}
		if (!codr) break;
		if (ch == '\n')
			break;
		cmd[ln++] = ch;
	}
	if (ln == 0) return 0;
	return 1;
}

void work(int fd, int fdg)
{
	char flag, ch, fdi;
	char stop = 0, nxt = 1;
	int codr, i, ok, cend;
	fdi = open(filein, O_RDONLY);
	if (fdi == -1)
	{
		perror("Eroare la deschiderea fisierului comenzi.sh");
		exit(55);
	}
	while (1)
	{
		ok = getNextCmd(fdi);
		if (!ok)
		{
			write(fd,&stop,1);
			return;
		}
		write(fd,&nxt,1);
		for (i=0;i<ln;++i)
			write(fd,&cmd[i],1);
		write(fd,&nxt,1);
		read(fdg, &cend, sizeof(cend));
		printf("Statusul comenzii rulate: %d\n", cend);
	}
}

int main(int argc, char *argv[])
{	
	int fd, fdo;
	char *pipeName = "socket";
	char *retRez=  "sendRet";
	if (-1 == mkfifo(pipeName, 0600) && errno != EEXIST)
	{
		perror("Eroare la crearea pipe ului socket");
		exit(1);
	}
	if (-1 == mkfifo(retRez, 0600) && errno != EEXIST)
	{
		perror("Eroare la crearea pipe ului sendRet");
		exit(2);
	}
	fd = open(pipeName, O_WRONLY);
	fdo = open(retRez, O_RDONLY);
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
