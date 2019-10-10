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

void initializare_fisier_date();
void afisare_fisier_date();
void secventa_de_actualizari(int n, char *nr[]);

int main(int argc, char *argv[])
{	
	if (argc == 1)
	{
		fprintf(stderr, "Eroare: programul trebuie apelat cu optiunile: -i, -o, sau cu o secventa de numere reale!\n");
		return 1;
	}

	if (!strcmp(argv[1], "-i"))
	{
		initializare_fisier_date();
		return 0;
	}
	
	if(!strcmp(argv[1], "-o"))
	{
		afisare_fisier_date();
		return 0;
	}

	secventa_de_actualizari(argc, argv);
	return 0;
}

void initializare_fisier_date()
{
	int fd;
	float val;

	printf("Dati cantitatea initiala de combustibil (i.e., un numar real pozitiv): ");
	while ( (1 != scanf("%f", &val)) && (val < 0) )
	{
		fprintf(stderr, "Eroare: nu ati introdus un numar real pozitiv\n");
	}

	if ( -1 == (fd = open("peco.bin", O_WRONLY | O_CREAT | O_TRUNC, 0600)))
	{
		perror("Eroare la crearea fisierului de date...");
		exit(2);
	}
	if ( -1 == write(fd, &val, sizeof(float)))
	{
		perror("Eroare la scrierea valorii initiale in fisierul de date...");
		exit(3);
	}

	close(fd);
}

void afisare_fisier_date()
{
	int fd;
	float val;

	if ( -1 == (fd = open("peco.bin", O_RDONLY)))
	{
		perror("Eroare la deschiderea pentru afisare a fisierului de date...");
		exit(4);
	}
	if ( -1 == read(fd, &val, sizeof(float)))
	{
		perror("Eroare la citirea valorii finale din fisierul de date...");
		exit(5);
	}

	close(fd);
	printf("Stocul final de combustibil este: %f litri de combustibil.\n", val);
}

void secventa_de_actualizari(int n, char* nr[])
{
	float valoare, stoc;
	int fd, i;
	struct flock lock, unlock;
	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = sizeof(float);

	unlock.l_type = F_UNLCK;
	unlock.l_whence = SEEK_SET;
	unlock.l_start = 0;
	unlock.l_len = sizeof(float);

	if ( -1 == (fd = open("peco.bin", O_RDWR) ) )
	{
		perror("Eroare la deschiderea pentru actualizare a fisierului de date...");
		exit(6);
	}

	srandom(getpid());

	for (i=1;i<n;++i)
	{
		if (1 != sscanf(nr[i], "%f", &valoare))
		{
			fprintf(stderr, "[PID: %d] Eroare: ati specificat o valoarea invalida drept argument de tip numar real : %s !\n", getpid(), nr[i]);
			exit(7);
		}

//		printf("[ProcesId:%d] Incerc sa pun lacatul\n", getpid());

		if (-1 == fcntl(fd, F_SETLKW, &lock))
		{
			exit(3);
		}

	//	printf("[ProcesId:%d] Lacat pus\n", getpid());

		if (-1 == lseek(fd, 0, SEEK_SET))
		{
			perror("Eroare la repozitionarea in fisierul de date, pentru citire...");
			exit(8);
		}
		if (-1 == read(fd, &stoc, sizeof(float)))
		{
			perror("Eroare la citirea valorii din fisierul de date...");
			exit(9);
		}

		printf("[PID: %d] Se adauga/extrage in/din rezervor cantitatea de %f litri de combustibil.\n", getpid(), valoare);

		stoc = stoc + valoare;

		if (stoc < 0)
		{
			printf("Stoc insuficient, operatie refuzata!\n");
			stoc = stoc - valoare;
		}

		if (-1 == lseek(fd, 0, SEEK_SET) )
		{
			perror("Eroare la repozitionarea in fisierul de date, pentru scriere...");
			exit(10);
		}
		if (-1 == write(fd, &stoc, sizeof(float)))
		{
			perror("Eroare la scrierea valorii actualizate in fisierul de date...");
			exit(11);
		}

		if (-1 == fcntl(fd, F_SETLKW, &unlock))
		{
			exit(4);
		}

	//	printf("[Procesid:%d] Lacat scos\n", getpid());

		sleep(random()%5);
	}
	
	close(fd);
	printf("[PID: %d] Am terminat de procesat secventa de actualizari.\n", getpid());
}
