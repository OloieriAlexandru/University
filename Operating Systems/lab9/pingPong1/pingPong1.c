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

int citesteFlag()
{
	// o sa incerc sa pun un lacat pe citire, ca cel care incearca sa scrie sa astepte
	struct flock lacat;
	int fd, codCitire;
	unsigned char flag;

	if (-1 == (fd = open("flag.bin", O_RDONLY)))
	{
		fprintf(stderr, "[ProcessId=%d] Eroare la deschiderea fisierului cu flag", getpid());
		exit(2);
	}

	lacat.l_type = F_RDLCK;
	lacat.l_whence = SEEK_SET;
	lacat.l_start = 0;
	lacat.l_len = 1;

	if (-1 == fcntl(fd, F_SETLKW, &lacat))
	{
		if (errno = EINTR)
			fprintf(stderr, "Apelul fcntl a fost intrerupt de un semnal");
		else
			fprintf(stderr, "Eroare unknown la blocaj");
		perror("\tMotivul");
		exit(3);
	}

	codCitire = read(fd, &flag, 1);

	if (codCitire == -1)
	{
		perror("Eroare la citire\n");
		exit(3);
	}

	lacat.l_type = F_UNLCK;
	if (-1 == fcntl(fd, F_SETLKW, &lacat))
	{
		if (errno = EINTR)
			fprintf(stderr, "Apelul fcntl a fost intrerupt de un semnal");
		else
			fprintf(stderr, "Eroare unknown la blocaj");
		perror("\tMotivul");
		exit(3);
	}
	close(fd);

	return (int)flag - '0';
}

void puneFlag(int f)
{
	struct flock lacat;
	int fd, codScriere;
	unsigned char flag = f + '0';

	if (-1 == (fd = open("flag.bin", O_WRONLY)))
	{
		fprintf(stderr, "[ProcessId=%d] Eroare la deschiderea fisierului cu flag", getpid());
		exit(2);
	}

	lacat.l_type = F_WRLCK;
	lacat.l_whence = SEEK_SET;
	lacat.l_start = 0;
	lacat.l_len = 1;

	if (-1 == fcntl(fd, F_SETLKW, &lacat))
	{
		if (errno = EINTR)
			fprintf(stderr, "Apelul fcntl a fost intrerupt de un semnal");
		else
			fprintf(stderr, "Eroare unknown la blocaj");
		perror("\tMotivul");
		exit(3);
	}

	codScriere = write(fd, &flag, 1);
	if (codScriere == -1)
	{
		perror("Eroare la scriere\n");
		exit(3);
	}

	lacat.l_type = F_UNLCK;
	if (-1 == fcntl(fd, F_SETLKW, &lacat))
	{
		if (errno = EINTR)
			fprintf(stderr, "Apelul fcntl a fost intrerupt de un semnal");
		else
			fprintf(stderr, "Eroare unknown la blocaj");
		perror("\tMotivul");
		exit(3);
	}
	close(fd);
}

void dialog_tata()
{
	int fd, flagCurent, cod;
	int maiAmReplici = 1;
	unsigned char ch;
	srandom(getpid());
	if (-1 == (fd = open("replici-parinte.txt", O_RDONLY)))
	{
		perror("Eroare la deschiderea fisierului cu replici al parintelui!\n");
		exit(2);
	}
	while(maiAmReplici)
	{
		flagCurent = citesteFlag();
		if (flagCurent == 0 || flagCurent == 2)
		{
			// citesc si afisez replica
			printf("Tata: ");
			while (1)
			{
				cod = read(fd, &ch, 1);
				if (cod == -1)
				{
					perror("Eroare la citire a replici fiului!");
					exit(3);
				}
				if (!cod)
				{
					maiAmReplici = 0;
					break;
				}
				if (ch == '\n')
				{
					printf("\n");
					break;
				}
				printf("%c",ch);
			}
			puneFlag(1);
		}
		sleep(random()%2+1);
	}
	close(fd);
	puneFlag(2);
}

void dialog_fiu()
{
	int fd, flagCurent;
	int maiAmReplici = 1, cod;
	unsigned char ch;
	srandom(getpid());
	if (-1 == (fd = open("replici-fiu.txt", O_RDONLY)))
	{
		perror("Eroare la deschiderea fisierului cu replici al fiului\n");
		exit(1);	
	}
	while(maiAmReplici)
	{
		flagCurent = citesteFlag();
		if (flagCurent == 1 || flagCurent == 2)
		{
			// citesc si afisez replica
			printf("Fiu: ");
			while (1)
			{
				cod = read(fd, &ch, 1);
				if (cod == -1)
				{
					perror("Eroare la citire a replici fiului!");
					exit(3);
				}
				if (!cod)
				{
					maiAmReplici = 0;
					break;
				}
				if (ch == '\n')
				{
					printf("\n");
					break;
				}
				printf("%c",ch);
			}
			puneFlag(0);
		}
		sleep(random()%2+1);
	}
	close(fd);
	puneFlag(2);
}

int main(int argc, char *argv[])
{	
	pid_t pid_fiu;

	puneFlag(0);

	if (-1 == (pid_fiu = fork()))
	{
		perror("Eroare la fork"); return 1;
	}
	if (pid_fiu == 0)
	{
		printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid());
		dialog_fiu();
	}
	else
	{
		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());
		dialog_tata();
	}
	printf("Sfarsitul execitiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte");
	return 0;
}
