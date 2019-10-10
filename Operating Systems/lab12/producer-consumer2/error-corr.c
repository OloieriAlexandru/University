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
char txt[nmax];
int ln;

int isLetter(char c)
{
	if (c>='A' && c<='Z')
	   c = c - 'A' + 'a';
	return (c >= 'a' && c<='z');
}

int isDigit(char c)
{
	return (c>='0' && c<='9');
}

int isAlphanumericCharacter(char c)
{
	return (isLetter(c) || isDigit(c));
}

void moveLeft(int poz)
{
	if (poz < 0 || poz >= ln) return;
	int i;
	// muta toate caracterele cu o pozitie in stanga, incepand de la pozitia poz
	for (i=poz;i<ln;++i)
		txt[i] = txt[i+1];
	--ln;
}

int moveRight(int poz)
{
	if (poz < 0 || poz > ln) return 0;
	if (ln == nmax) return 0;
	// muta toate caracterele cu o pozitie in dreapta, incepand de la pozitia poz
	int i;
	for (i = ln;i>=poz;--i)
		txt[i+1] = txt[i];
	++ln;
	return 1;
}

int main(int argc, char *argv[])
{	
	int fd, fd2, codr, i;
	char *pip = "p1p2", ch;
	if (-1 == mkfifo(pip,0600) && errno != EEXIST)
	{
		perror("Eroare la crearea pipe ului p1p2");
		exit(5);
	}

	fd = open(pip,O_WRONLY);
	if (-1 == fd)
	{
		perror("Eroare la deschiderea pipe ului p1p2");
		exit(6);
	}

	fd2 = open("text.txt", O_RDONLY);
	if (fd2 == -1)
	{
		perror("Eroare la deschiderea fisierului text.txt. Motivul");
		exit(7);
	}
	while (1)
	{
		codr = read(fd2, &ch, 1);
		if (codr == -1)
		{
			perror("Eroare la citirea din fisierul text.txt");
			exit(8);
		}
		if (!codr) break;
		txt[ln++] = ch;
		if (ln == nmax)
		{
			txt[ln-2] = '\n';
			txt[ln-1] = 0;
			break;
		}
	}
	if (ln < nmax && ln > 0 && txt[ln-1] != '\n')
	   txt[ln++] = '\n';
	close(fd2);

	for (i=0;i<ln;++i)
	{
		if (txt[i] != ' ') continue;
		while (i<(ln-1) && txt[i+1] == ' ')
			moveLeft(i+1);
	}
	for (i=0;i<ln;++i)
	{
		if (isAlphanumericCharacter(txt[i]) || txt[i] == ' ' || txt[i] == '\n' || txt[i] == '\t') continue;
		if (i<ln-1)
		{
			if (moveRight(i+1))
			{
				txt[i+1] = ' ';
			}
		}
	}

	for (i=0;i<ln;++i)
		write(fd,&txt[i],1);
	close(fd);
	return 0;
}
