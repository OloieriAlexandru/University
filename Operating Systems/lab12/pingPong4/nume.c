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
#define nmax 105

int readLine(int fd, char *buff)
{
	char ch;
	int ln = 0, codR;
	memset(buff, 0, sizeof(buff));
	while (1)
	{
		codR = read(fd, &ch, 1);
		if (codR == -1)
		{
			perror("Eroare la citirea din fisier!\n");
			exit(1);
		}
		if (codR == 0 || (ch == '\n' && ln == 0))
			return 0;
		if (ch == '\n')
			return 1;
		buff[ln++] = ch;
	}
	return 0;
}

int main(int argc, char *argv[])
{	
	int fd, fdi, fdo, rsp, cod;
	int stopped = 0;
	char line[nmax];
	char stop = 1, ch = 0, ms;

	if (-1 == mkfifo("num2tel",0600))
	{
		if (errno != EEXIST)
		{
			perror("Eroare la crearea pipe ului num2tel");
			exit(2);
		}
	}
	
	fdo = open("num2tel",O_WRONLY);
	fdi = open("tel2num",O_RDONLY);
	fd = open("nume.txt",O_RDONLY);

	while (1)
	{
		rsp = readLine(fd, line);

		if (rsp)
		{
			printf("%s - ", line);
			fflush(stdout);
		}

		if (!stopped && rsp)
		{
			write(fdo,&ch,1);
			cod = read(fdi,&ms,1);
			if (ms == stop || cod == 0)
				stopped = 1;
		}

		if (!rsp) break;
	}
	if (!stopped)
		write(fdo,&stop,1);
	close(fdi);
	close(fdo);
	close(fd);
	return 0;
}
