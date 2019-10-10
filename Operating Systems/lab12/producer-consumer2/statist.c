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

char getLastChar(int poz)
{
	int i;
	for (i = poz-1;i>=0;--i)
	{
		if (txt[i] != ' ') return txt[i];
	}
	return 0;
}

int main(int argc, char *argv[])
{	
	int fd, codr, i, nrLinii = 0;
	char *pip = "p2p3", ch, *oFile = "text_corectat.txt";
	if (-1 == mkfifo(pip,0600) && errno != EEXIST)
	{
		perror("Eroare la crearea pipe ului p2p3");
		exit(5);
	}
	fd = open(pip,O_RDONLY);
	if (fd == -1)
	{
		perror("Eroare la deschiderea pipe ului p2p3");
		exit(6);
	}
	
	while(1)
	{
		codr = read(fd, &ch, 1);
		if (codr == -1)
		{
			perror("Eroare la citirea din pipe ul p2p3");
			exit(197);
		}
		if (!codr) break;
		if (ch == '\n') ++nrLinii;
		txt[ln++] = ch;
	}
	close(fd);

	fd = open(oFile, O_CREAT | O_WRONLY | O_TRUNC,0600);
	if (fd == -1)
	{
		perror("Eroare la crearea fisierului text_corectat.txt");
		exit(157);
	}

	for (i=0;i<ln;++i)
	{
		ch = getLastChar(i);
		if (ch == 0 && i != 0) continue;
		if (ch == '\n')
		{
	 		if (txt[i] != '\t' && moveRight(i))
			{
				txt[i] = '\t';
			}
		}
		else
		if (ch == 0 && i == 0)
		{
			if (txt[i] != '\t' && moveRight(i))
				txt[i] = '\t';
		}
	}

	for (i=0;i<ln;++i)
		write(fd, &txt[i],1);
	close(fd);

	FILE *fout=fopen("statistica.txt","w");
	fprintf(fout, "Numarul de linii: %d\nNumarul de caractere: %d\n", nrLinii, ln);
	fclose(fout);

	return 0;
}
