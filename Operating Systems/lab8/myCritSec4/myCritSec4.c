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

char fileName[] = "instr";

int executaOperatie(int fd, int cheie, int valoare)
{
	// citesc valorile din fisierul indicat de fileDescriptorul=fd si caut o pereche cu cheia 'cheie', daca nu gasesc o adaug la final
	// functia returneaza -1 daca a fost vreo eroare
	struct flock lock;
	struct flock unlock;
	int c, v, codRead;
	int gasit = 0;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_start = -2*sizeof(int);
	lock.l_len = 2*sizeof(int);

	unlock.l_type = F_UNLCK;
	unlock.l_whence = SEEK_CUR;
	unlock.l_start = -2*sizeof(int);
	unlock.l_len = 2*sizeof(int);

	if (-1 == lseek(fd, 0L, SEEK_SET))
	{
		fprintf(stderr, "Eroare la repozitionarea cursorului la inceputul fisierului asupra caruia se executa operatii\n");
		return -1;
	}
	while (0 != (codRead = read(fd, &c, sizeof(int))))
	{
		if (codRead == -1)
		{
			fprintf(stderr, "Eroare la citirea cheii unei perechi\n");
			return -1;
		}
		codRead = read(fd, &v, sizeof(int));
		if (!codRead) break;
		if (codRead == -1)
		{
			fprintf(stderr, "Eroare la citirea valorii unei perechi\n");
			return -1;
		}
//		printf("[PID: %d] Incerc punerea lacatului\n", getpid());
		if (-1 == fcntl(fd, F_SETLKW, &lock))
		{
			fprintf(stderr, "Eroare la punerea lacatului!\n");
			return -1;
		}
//		printf("[PID: %d] Lacat pus cu succes\n", getpid());
		if (-1 == lseek(fd, -2*sizeof(int), SEEK_CUR))
		{
			fprintf(stderr, "Eroare la repozitionarea cursorului!\n");
			return -1;
		}

		codRead = read(fd, &c, sizeof(int));
		if (codRead == -1)
		{
			fprintf(stderr, "Eroare la recitirea primului numar din pereche!\n");
			return -1;
		}
		codRead = read(fd, &v, sizeof(int));
		if (codRead == -1)
		{
			fprintf(stderr, "Eroare la recitirea primului numar din pereche!\n");
			return -1;
		}

		if (c == cheie)
		{
			printf("[PID: %d] Am gasit o pereche cu cheia %d, valoarea curenta: %d, noua  valoare: %d\n", getpid(),cheie, v, v+valoare);
			gasit = 1;
			v += valoare;
			if (-1 == lseek(fd, -sizeof(int), SEEK_CUR))
			{
				fprintf(stderr, "Eroare la repozitionarea cursorului pentru inlocuirea valorii!\n");
				return -1;
			}
			if (-1 == write(fd, &v, sizeof(int)))
			{
				fprintf(stderr, "Eroare la scrierea valorii din pereche in fisier\n");
				return -1;
			}

		}
		if (-1 == fcntl(fd, F_SETLKW, &unlock))
		{
			fprintf(stderr, "Eroare la scoaterea lacatului!\n");
			return -1;
		}
		if (gasit) break;
	}
	if (!gasit)
	{
		printf("[PID: %d] Nu am gasit o pereche cu cheia %d in fisier, adaug eu (%d,%d) la finalul acestuia\n", getpid(), cheie, cheie, valoare);
		lock.l_start = 0;
		unlock.l_start = 0;
		if (-1 == fcntl(fd, F_SETLKW, &lock))
		{
			fprintf(stderr, "Eroare la punerea lacatului la finalul fisierului, pentru adaugarea unei valori noi\n");
			return -1;
		}

		if (-1 == write(fd, &cheie, sizeof(int)))
		{
			fprintf(stderr, "Eroare la adaugarea cheii la finalul fisierului\n");
			return -1;
		}
		if (-1 == write(fd, &valoare, sizeof(int)))
		{
			fprintf(stderr, "Eroare la adaugarea valorii la finalul fisierului\n");
			return -1;
		}

		if (-1 == fcntl(fd, F_SETLKW, &unlock))
		{
			fprintf(stderr, "Eroare la scoaterea lacatului de la finalul fisierului\n");
			return -1;
		}
	}
}

void executaOperatii(char *numeFisier, int fdo, char *fileIn)
{
	int cheie, valoare;
	int codRead;
	int fd;

	srandom(getpid());

	if (-1 == (fd = open(numeFisier, O_RDWR)))
	{
		fprintf(stderr, "Eroare la deschiderea fisierului %s\n", numeFisier);
		perror("\tMotivul");
		close(fdo);
		free(fileIn);
		exit(4);
	}

	printf("[PID: %d] Incep efectuarea operatiilor\n", getpid());

	while (0!=(codRead = read(fdo, &cheie, sizeof(int))))
	{
		if (-1 == codRead)
		{
			free(fileIn);
			close(fd);
			close(fdo);
			perror("Eroare la citirea cheii dintr-o pereche");
			exit(5);
		}
		codRead = read(fdo, &valoare, sizeof(int));
		if (!codRead) break;
		if (-1 == codRead)
		{
			free(fileIn);
			close(fd);
			close(fdo);
			perror("Eroare la citirea valorii dintr-o pereche");
			exit(6);
		}
		printf("[PID: %d] Executa o operatie: (%d,%d)\n", getpid(), cheie, valoare);
		if (-1 == executaOperatie(fd, cheie, valoare))
		{
			free(fileIn);
			close(fd);
			close(fdo);
			exit(7);
		}
		sleep(rand()%2+1);
	}

	close(fd);
}

int main(int argc, char *argv[])
{	
	int n, i, fd;
	int cheie, valoare;
	int codRead;
	if (argc <= 2)
	{
		printf("Trebuie sa specifici 2 argumente: numele fisierului asupra caruia se executa operatiile si indicele fisierului din care se citesc operatiile\n");
		return 0;
	}
	int nr;
	if (1 != sscanf(argv[2], "%d",&nr))
	{
		fprintf(stderr, "[PID: %d] Eroare: ati specificat o valoarea invalida drept argument", getpid());
		exit(1);
	}
	int ln = 0, cnr = nr;
	while (cnr) ++ln, cnr/=10;
	int newLen = strlen(fileName) + ln;
	char *newFileName = (char*) malloc(newLen);
	memset(newFileName, 0, sizeof(newFileName));
	strcpy(newFileName, fileName);
	for (i=newLen-1;nr;--i)
		newFileName[i] = (nr%10 + '0'), nr/=10;	

	if (-1 == (fd = open(newFileName, O_RDONLY)))
	{
		printf("Eroare la crearea fisierului %s\n", newFileName);
		perror("Motivul");
		free(newFileName);
		return 1;
	}	

	executaOperatii(argv[1], fd, newFileName);

	free(newFileName);
	close(fd);
	return 0;
}
