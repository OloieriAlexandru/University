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
	int fd, codRead, codBlocaj;
	char ch;
	struct flock lacat;

	if (argv[1] == NULL)
	{
		fprintf(stderr, "Programul trebuie apelat cu cel putin un parametru.\n");
		exit(1);
	}

	if (-1 == (fd = open("fis.dat", O_RDWR)) )
	{
		perror("Nu pot deschide fisierul fis.dat deoarece ");
		exit(2);
	}

	lacat.l_type = F_WRLCK;
	lacat.l_whence = SEEK_SET;
	lacat.l_start = 0;
	lacat.l_len = 1;

	if (-1 == fcntl(fd, F_SETLKW, &lacat))
	{
		if (errno == EINTR)
			fprintf(stderr, "Apelul fcntl a fost intrerupt de un semnal [ProcesId:%d].\n", getpid());
		else
			fprintf(stderr, "Eroare unknown la blocaj [ProcesId:%d].\n", getpid());
		perror("\tMotivul");
		exit(3);
	}
	else
		printf("Blocaj reusit [ProcesId:%d].\n", getpid());

	while (0!=(codRead = read(fd, &ch, 1)) )
	{
		if (-1 == codRead)
		{
			fprintf(stderr,"errno = %d: ", errno);
			perror("eroare la apelul read");
			exit(4);
		}

		if (ch == '#')
		{
			if (-1 == lseek(fd, -1L, 1)) {perror("Eroare la apelul lseek"); exit(5);}
			sleep(3);
			if (-1 == write(fd, argv[1], 1)) {perror("Eroare la apelul write"); exit(6);}
			printf("Terminat. S-a inlocuit primul # gasit in fisierul fis.dat [ProcesId:%d].\n", getpid());
			return 0;
		}
	}

	close(fd);
	printf("Terminat. Nu exista # in fisierul fis.dat [ProcesId:%d].\n", getpid());

	return 0;
}
