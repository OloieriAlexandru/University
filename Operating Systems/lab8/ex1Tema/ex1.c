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

int main(int argc, char *argv[])
{	
	if (argc < 3)
	{
		printf("Trebuie sa introduci 2 argumente: un sir de caractere si alt caracter\n");
		exit(1);
	}
	int fd;
	struct flock lck;
	struct flock unlock;
	unsigned char ch;
	char oth = argv[2][0];
	int codRead;

	lck.l_type = F_WRLCK;
	lck.l_whence = SEEK_CUR;
	lck.l_start = 0;
	lck.l_len = 1;

	unlock.l_type = F_UNLCK;
	unlock.l_whence = SEEK_CUR;
	unlock.l_start = -1;
	unlock.l_len = 1;

	if (-1 == (fd = open("text.dat", O_RDWR)))
	{
		printf("Eroare la deschiderea fisierului \"text.dat\"\n");
		exit(2);
	}
	while (0 != (codRead = read(fd, &ch, 1)))
	{
		if (-1 == codRead)
		{
			perror("Eroare la apelul read");
			exit(3);
		}
		if (strchr(argv[1], ch))
		{
			unsigned char old = ch;
			if (-1 == lseek(fd, -1L, 1))
			{
				perror("Eroare la apelul seek");
				exit(4);
			}

			printf("Pun blocant lacatul pe caracterul '%c' gasit [Proces:%d].\n", ch, getpid());

			if (-1 == fcntl(fd, F_SETLKW, &lck))
			{
				fprintf(stderr, "Eroare la blocaj [ProcesId:%d]\n", getpid());
				perror("\tMotivul");
				exit(5);
			}
			else printf("Blocaj reusit [ProcesId:%d].\n", getpid());

			codRead = read(fd, &ch, 1);
			if (codRead == -1)
			{
				perror("Eroare la apelul read");
				exit(3);
			}

			if (strchr(argv[1], ch))
			{
				if (-1 == lseek(fd, -1L, 1))
				{
					perror("Eroare la apelul seek");
					exit(6);
				}
			}
			else
			{
				printf("Caracterul nu mai este %c, scot blocajul si caut in continuare!\n",old);
				if (-1 == fcntl(fd, F_SETLKW, &unlock))
				{
					fprintf(stderr, "Eroare la blocaj [ProcesId:%d]\n", getpid());
					perror("\tMotivul");
					exit(7);
				}
				continue;
			}
			if (-1 == write(fd, &oth, 1))
			{
				perror("Eroare la apelul write");
				exit(8);
			}

			if (-1 == fcntl(fd, F_SETLKW, &unlock))
			{
				fprintf(stderr, "Eroare la blocaj [ProcesId:%d]\n", getpid());
				exit(6);
			}
		}
	}

	close(fd);
	printf("Am terminat de inlocuit!\n");
	return 0;
}
