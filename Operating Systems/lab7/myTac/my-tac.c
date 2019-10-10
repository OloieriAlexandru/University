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

int getLine(int fd)
{
	char ch;
	while (1)
	{
		if (-1 == lseek(fd,-1,SEEK_CUR))
		{
			perror("Eroare la lseek");
			exit(127);
		}
		read(fd,&ch,1);
		if (ch == '\n')
		{
			break;
		}
		printf("%c",ch);
	}
printf("\n");
}

void work(int fd1, int fd2)
{
	int ok;
	char ch;
	if (-1 == lseek(fd1, 0, SEEK_END))
	{
		perror("Eroare la mutarea cursorului la finalul fisierului sursa");
		exit(10);
	}
	lseek(fd1,-1,SEEK_CUR);
	if (ch == '\n')
		lseek(fd1,-1,SEEK_CUR);
	while(1)
	{
		ok = getLine(fd1);
		if (!ok) break;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "Trebuie sa specifici 2 argumente: fisier_sursa fisier");
		exit(10);
	}
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1)
	{
		perror("Eroare la deschiderea fisierului sursa\n");
		exit(1);
	}
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
	if (fd2 == -1)
	{
		perror("Eroare la crearea fisierului destinatie\n");
		exit(2);
	}
	work(fd1, fd2);
	close(fd1);
	close(fd2);
	return 0;
}
