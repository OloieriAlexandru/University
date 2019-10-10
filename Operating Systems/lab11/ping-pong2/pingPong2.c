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

void printTxt(char *txt)
{
	int fd = open("persoane.txt", O_WRONLY), ln, i;
	char ch;
	if (-1 == lseek(fd, 0, SEEK_END))
	{
		perror("Eroare la lseek");
		exit(99);
	}
	ln = strlen(txt);
	for (i = 0; i<ln; ++i)
	{
		ch = txt[i];
		write(fd, &ch, 1);
	}
	close(fd);
}

int main(int argc, char *argv[])
{	
	int cp1p2[2], cp2p1[2];
	pid_t pid1;
	unsigned char oct = 0;
	if (-1 == pipe(cp1p2))
	{
		perror("Eroare la primul apel pipe()\n");
		exit(1);
	}
	if (-1 == pipe(cp2p1))
	{
		perror("Eroare la al doilea apel pipe()\n");
		exit(2);
	}
	if (-1 == (pid1 = fork()))
	{
		perror("Eroare la primul apel fork()");
		exit(3);
	}
	int fd = creat("persoane.txt",0600);
	close(fd);
	if (pid1 == 0)
	{
		// fiul 1
		char stop = 1;
		int codr, ln, stopped = 0, tipBr;
		char nume[nmax], ch;
		fd = open("nume.txt",O_RDONLY);
		close(cp1p2[0]);
		close(cp2p1[1]);
		while (1)
		{
			memset(nume,0,sizeof(nume));
			ln = 0;
			while (1)
			{
				codr = read(fd,&ch,1);
				if (-1 == codr)
				{
					perror("Eroare la citirea din fisierul nume.txt");
					exit(100);	
				}
				if (!codr)
				{
					tipBr = 2;
					break;
				}
				if (ch == '\n')
				{
					tipBr = 1;
					break;
				}
				nume[ln++] = ch;
			}
			if (tipBr == 1 && !stopped)
				nume[ln++]=' ';
			else
				nume[ln++]='\n';
			if (!stopped)
			{
				codr = read(cp2p1[0],&ch,1);
				if (codr == -1)
				{
					perror("Eroare la citirea din canalul cp2p1");
					exit(101);
				}
				if (ch == stop)
					stopped = 1;
			}
			printTxt(nume);
			if (!stopped && tipBr != 2)
				write(cp1p2[1],&oct,1);
			if (tipBr == 2) break;
		}
		close(cp2p1[0]);
		write(cp1p2[1],&stop,1);
		close(cp1p2[1]);
		close(fd);
	}
	else
	{
		pid_t pid2;
		if (-1 == (pid2 = fork()))
		{
			perror("Eroare la al doilea apel fork()\n");
			exit(4);
		}
		if (pid2 == 0)
		{
			// fiul 2
			char stop = 1, ch;
			int ln, codr, stopped = 0, tipBr;
			char prenume[nmax];
			fd = open("prenume.txt",O_RDONLY);
			close(cp1p2[1]);
			close(cp2p1[0]);
			while (1)
			{
				memset(prenume, 0, sizeof(prenume));
				ln = 0;
				while (1)
				{
					codr = read(fd,&ch,1);
					if (codr == -1)
					{
						perror("Eroare la citirea din fisierul prenume.txt");
						exit(102);
					}
					if (!codr)
					{
						tipBr = 2;
						break;
					}
					if (ch == '\n')
					{
						tipBr = 1;
						break;
					}
					prenume[ln++] = ch;
				}
				prenume[ln++] = '\n';
				if (!stopped)
				{
					codr = read(cp1p2[0],&ch,1);
					if (codr == -1)
					{
						perror("Eroare la citirea din canalul cp1p2");
						exit(103);
					}
					if (ch == stop)
						stopped = 1;
				}
				printTxt(prenume);
				if (!stopped && tipBr != 2)
					write(cp2p1[1],&oct,1);
				if (tipBr == 2) break;
			}
			close(cp1p2[0]);
			write(cp2p1[1],&stop,1);
			close(cp2p1[1]);
			close(fd);
		}
		else
		{
			// parintele
			write(cp2p1[1],&oct,1);

			usleep(1);

			close(cp2p1[0]);
			close(cp2p1[1]);
			close(cp1p2[0]);
			close(cp1p2[1]);

			int i;
			for (i=1;i<=2;++i)
				wait(NULL);
			printf("[Parintele] Fii au terminat treaba\n");
		}
	}
	return 0;
}
