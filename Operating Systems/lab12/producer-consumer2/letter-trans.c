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

int whiteChar(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int finalChar(char c)
{
	return (c == '.' || c == '?' || c == '!');
}

char getLastNonWhiteChar(int poz)
{
	int i;
	for (i = poz-1;i>=0;--i)
	{
		if (!whiteChar(txt[i])) return txt[i];
	}
	return 0;
}

int isSmallLetter(char c)
{
	return (c>='a' && c<='z');
}

int main(int argc, char *argv[])
{	
	int fd1, fd2, codr, i;
	char ch, *pip1 = "p1p2", *pip2 = "p2p3";
	
	if (-1 == mkfifo(pip1,0600) && errno != EEXIST)
	{
		perror("Eroare la crearea pipe ului p1p2");
		exit(6);
	}
	if (-1 == mkfifo(pip2,0600) && errno != EEXIST)
	{
		perror("Eroare la crearea pipe ului p2p3");
		exit(7);
	}	

	fd1 = open(pip1,O_RDONLY);
	if (fd1 == -1)
	{
		perror("Eroare la deschiderea pipe ului p1p2");
		exit(8);
	}
	fd2 = open(pip2,O_WRONLY);
	if (fd2 == -1)
	{
		perror("Eroare la deschiderea pipe ului p2p3");
		exit(9);
	}

	while(1)
	{
		codr = read(fd1,&ch,1);
		if (codr == -1)
		{
			perror("Eroare la citirea din pipe ul p1p2");
			exit(10);
		}
		if (!codr) break;
		txt[ln++] = ch;
	}
	close(fd1);
	for (i = 0;i<ln;++i)
	{
		ch = getLastNonWhiteChar(i);
		if (!ch) continue;
		if (finalChar(ch) && isSmallLetter(txt[i]))
		   txt[i] = txt[i] - 'a' + 'A';	
	}
	for (i=0;i<ln;++i)
		write(fd2,&txt[i],1);
	close(fd2);
	return 0;
}
