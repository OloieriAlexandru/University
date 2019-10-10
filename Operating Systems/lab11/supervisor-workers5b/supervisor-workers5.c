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

int main(int argc, char *argv[])
{	
	pid_t pid1;
	
	if (-1 == mkfifo("p0p1",0600))
		if (errno != EEXIST)
		{
			perror("Eroare la creare pipe: p0p1\n");
			exit(1);
		}

	if (-1 == mkfifo("p1p2",0600))
		if (errno != EEXIST)
		{
			perror("Eroare la creare pipe: p1p2\n");
			exit(2);
		}

	if (-1 == mkfifo("p2p0",0600))
		if (errno != EEXIST)
		{
			perror("Eroare la creare pipe: p2p0\n");
			exit(3);
		}

	if (-1 == (pid1 = fork()))
	{
		perror("Eroare la primul apel fork()");
		exit(4);
	}
	if (pid1 == 0)
	{
		// procesul fiu1
		printf("Fiul 1 la datorie, astept sa primesc informatii de la tatal meu\n");
		int codr;
		unsigned char ch;
		int fdi = open("p0p1",O_RDONLY);
		int fdo = open("p1p2",O_WRONLY);
		while (1)
		{
			codr = read(fdi,&ch,1);
			if (codr == -1)
			{
				printf("Fiu 1: eroare la citirea din canalul p0p1\n");
				exit(10);
			}
			if (!codr) break;
			if (ch>='A' && ch<='Z')
			   write(fdo,&ch,1);
		}
		printf("Gata boss, eu am primit tot si i-am trimis mai departe fiului 2, acum e treaba lui ce face\n");
		// mi-am terminat treaba, am selectat literele mari si le am trimis fiului 2
		close(fdi);
		close(fdo);
	}
	else
	{
		pid_t pid2;
		if (-1 == (pid2 = fork()))
		{
			perror("Eroare la al doilea apel fork()");
			exit(5);
		}
		if (pid2 == 0)
		{
			printf("Eu astept de la fiul 1 sa primesc literele\n");
			// procesul fiu2
			int codr, nrd = 0;
			unsigned char ch;
			int fv[26];
			memset(fv, 0, sizeof(fv));
			int fdi = open("p1p2",O_RDONLY);
			int fdo = open("p2p0",O_WRONLY);
			while (1)
			{
				codr = read(fdi,&ch,1);
				if (codr == -1)
				{
					printf("Fiu 2: eroare la citirea din canalul p1p2\n");
					exit(11);
				}
				if (!codr) break;
				if (!fv[ch-'A'])++nrd;
				++fv[ch-'A'];
			}
			close(fdi);
			write(fdo,&nrd,sizeof(int));
			close(fdo);
			FILE *fout=fopen("statistica.txt","w");
			for (int i=0;i<26;++i)
				if (fv[i])
					fprintf(fout,"%c: %d\n",(char)(i+'A'),fv[i]);
			fclose(fout);
		}
		else
		{
			// parintele
			printf("Parintele a inceput treaba\n");
			int dist = 0,codr,fd = open("date.txt",O_RDONLY);
			unsigned char ch;
			int fdo = open("p0p1",O_WRONLY);
			int fdi = open("p2p0",O_RDONLY);
			if (-1 == fd)
			{
				perror("Eroare la deschiderea fisierului date.txt in parinte");
				exit(6);
			}
			while (1)
			{
				codr = read(fd,&ch,1);
				if (codr == -1)
				{
					perror("Eroare la citire din fisierul date.txt");
					exit(7);
				}
				if (!codr) break;
				write(fdo,&ch,1);
			}
			printf("I-am dat tot primului fiu\n");
			close(fdo);
			codr = read(fdi,&dist,sizeof(int));
			printf("Numarul de litere mari distincte: %d\n", dist);
			close(fdi);
		}
	}
	return 0;
}
