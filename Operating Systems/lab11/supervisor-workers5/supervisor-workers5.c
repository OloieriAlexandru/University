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
	int c_p0p1[2],c_p1p2[2],c_p2p0[2];

	if (-1 == pipe(c_p0p1))
	{
		perror("Eroare la primul apel pipe()");
		exit(1);
	}
	if (-1 == pipe(c_p1p2))
	{
		perror("Eroare la al doilea apel pipe()");
		exit(2);
	}
	if (-1 == pipe(c_p2p0))
	{
		perror("Eroare la al treilea apel pipe()");
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
		close(c_p2p0[0]);
		close(c_p2p0[1]);
		close(c_p1p2[0]);
		close(c_p0p1[1]);
		int codr;
		unsigned char ch;
		while (1)
		{
			codr = read(c_p0p1[0],&ch,1);
			if (codr == -1)
			{
				printf("Fiu 1: eroare la citirea din canalul p0p1\n");
				exit(10);
			}
			if (!codr) break;
			if (ch>='A' && ch<='Z')
			   write(c_p1p2[1],&ch,1);
		}
		printf("Gata boss, eu am primit tot si i-am trimis mai departe fiului 2, acum e treaba lui ce face\n");
		close(c_p0p1[0]);
		close(c_p1p2[1]);
		// mi-am terminat treaba, am selectat literele mari si le am trimis fiului 2
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
			close(c_p0p1[0]);
			close(c_p0p1[1]);
			close(c_p2p0[0]);
			close(c_p1p2[1]);
			int codr, nrd = 0;
			unsigned char ch;
			int fv[26];
			memset(fv, 0, sizeof(fv));
			while (1)
			{
				codr = read(c_p1p2[0],&ch,1);
				if (codr == -1)
				{
					printf("Fiu 2: eroare la citirea din canalul p1p2\n");
					exit(11);
				}
				if (!codr) break;
				if (!fv[ch-'A'])++nrd;
				++fv[ch-'A'];
			}
			close(c_p1p2[0]);
			write(c_p2p0[1],&nrd,sizeof(int));
			close(c_p2p0[1]);
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
			close(c_p0p1[0]);
			close(c_p2p0[1]);
			close(c_p1p2[0]);
			close(c_p1p2[1]);
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
				write(c_p0p1[1],&ch,1);
			}
			printf("I-am dat tot primului fiu\n");
			close(c_p0p1[1]);
			codr = read(c_p2p0[0],&dist,sizeof(int));
			close(c_p2p0[0]);
			printf("Numarul de litere mari distincte: %d\n", dist);
		}
	}
	return 0;
}
