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
#define MAXV1 15000
#define MAXV2 3

void generarePerechiNumere(int n)
{
	int i, v1, v2;
	int fd;

	if (-1 == (fd = open("operanzi.bin", O_TRUNC | O_CREAT | O_WRONLY, 0600)))
	{
		perror("Eroare la crearea fisierului \"operanzi.bin\"");
		exit(4);
	}

	srandom(getpid());
	for (i=1;i<=n;++i)
	{
		v1 = rand() % MAXV1;
		v2 = rand() % MAXV1;

		if (-1 == write(fd, &v1, sizeof(v1)))
		{
			perror("Eroare la scrierea primului numar dintr-o pereche");
			exit(5);
		}
		if (-1 == write(fd, &v2, sizeof(int)))
		{
			perror("Eroare la scrierea celui de-al doilea numar dintr-o pereche");
			exit(6);
		}
	}

	close(fd);
}

void generareCaractere(int n)
{
	int i, act;
	int fd;
	unsigned char ch;

	if (-1 == (fd = open("operatori.txt", O_WRONLY | O_TRUNC | O_CREAT, 0600)))
	{
		perror("Eroare la crearea fisierului \"operatori.txt\"");
		exit(2);
	}

	srandom(getpid());
	for (i=1;i<=n;++i)
	{
		act = rand()%MAXV2;
		switch(act)
		{
			case 0: ch = '+'; break;
			case 1: ch = '-'; break;
			case 2: ch = '*'; break;
			default: break;
		}
		if (-1 == write(fd, &ch, 1))
		{
			perror("Eroare la scrierea unui operator in fisier");
			exit(3);
		}	
	}
	close(fd);
}

void calcul(int n)
{
	int v1, v2, fd1, fd2, i, ans;
	int codRead;
	unsigned char ch;

	if (-1 == (fd1 = open("operanzi.bin",O_RDONLY)))
	{
		perror("Eroare la deschiderea fisierului \"operanzi.bin\" de catre tata");
		exit(1);
	}
	if (-1 == (fd2 = open("operatori.txt",O_RDONLY)))
	{
		perror("Eroare la deschiderea fisierului \"operatori.txt\" de catre tata");
		exit(2);
	}

	for (i=1;i<=n;++i)
	{
		codRead = read(fd1, &v1, sizeof(int));
		if (codRead == -1)
		{
			perror("Eroare la citirea primului numar dintr-o pereche de operanzi de catre parinte!");
			exit(3);
		}
		if (!codRead) break;
		codRead = read(fd1, &v2, sizeof(int));
		if (codRead == -1)
		{
			perror("Eroare la citirea celui de-al doilea numar dintr-o pereche de operanzi de catre parinte!");
			exit(4);
		}
		if (!codRead) break;
		codRead = read(fd2, &ch, sizeof(unsigned char));
		if (codRead == -1)
		{
			perror("Eroare la citirea unui operator de catre parinte!\n");
			exit(5);
		}
		if (!codRead) break;
		switch(ch)
		{
			case '+': ans = v1 + v2; break;
			case '-': ans = v1 - v2; break;
			case '*': ans = v1 * v2; break;
			default: break;
		}
		printf("Operatia %d: %d%c%d=%d\n",i,v1,ch,v2,ans);
	}

	close(fd1);
	close(fd2);
}

void printOperanzi()
{
	int fd, v1, v2, i;
	int codRead;
	if (-1 == (fd = open("operanzi.bin", O_RDONLY)))
	{
		fprintf(stderr, "Eroare la deschiderea fisierului \"operanzi.bin\"\n");
		exit(1);
	}
	i=0;
	while (0 != (codRead = read(fd, &v1, sizeof(int))))
	{
		if (codRead == -1)
		{
			fprintf(stderr, "Eroare la citirea din fisier a primei valori dintr-o pereche\n");
			exit(2);
		}
		codRead = read(fd, &v2, sizeof(int));
		if (!codRead) break;
		if (codRead == -1)
		{
			fprintf(stderr, "Eroare la citirea din fisier a celei de-a doua valori dintr-o pereche\n");
			exit(3);
		}	
		++i;
		printf("Pereche %d: (%d,%d)\n",i,v1,v2);
	}

	close(fd);
}

int main(int argc, char *argv[])
{	
	pid_t pid_f1, pid_f2;
	int n;

	if (argc >=2)
	{
		if (!strcmp(argv[1], "-o"))
		{
			printOperanzi();
			return 0;
		}
	}

	printf("N=");
	scanf("%d",&n);
	if (n<=0)
	{
		fprintf(stderr,"Trebuie sa introduci un numar intreg pozitiv!\n");
		return 1;
	}

	printf("[PID: %d] Procesul tata\n", getpid());

	if (-1 == (pid_f1 = fork()))
	{
		perror("Eroare la fork #1\n");
		return 2;
	}
	if (pid_f1 == 0)
	{
		printf("[PID: %d, PIDtata: %d] Fiu #1, generez perechi de numere\n", getpid(), getppid());
		generarePerechiNumere(n);
		return 0;
	}
	else
	{
		if (-1 == (pid_f2 = fork()))
		{
			perror("Eroare la fork #2\n");
			return 3;
		}
		if (pid_f2 == 0)
		{		
			printf("[PID: %d, PIDtata: %d] Fiu #2, generez caractere din multimea {+,-,*}\n", getpid(), getppid());
			generareCaractere(n);
			return 0;
		}
		else
		{
			int i;
			for (i=1;i<=2;++i)
			     wait(NULL);
			calcul(n);
			return 0;
		}
	}
	return 0;
}
