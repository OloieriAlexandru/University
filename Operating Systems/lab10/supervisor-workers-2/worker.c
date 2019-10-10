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
	int nr, sum = 0;
	FILE *fd;

	if (NULL == (fd = fopen(argv[1], "r")))
	{
		fprintf(stderr, "Worker: eroare la deschiderea fisierului de intrare %s.\n", argv[1]);
		return 1;
	}
	while (! feof(fd) )
	{
		if (1 == fscanf(fd, "%d",&nr))
			sum+=nr;
	}
	fclose(fd);
	if (NULL == (fd = fopen(argv[2], "w")))
	{
		fprintf(stderr, "Worker: eroare la deschiderea fisierului de iesire %s.\n", argv[2]);
		return 2;
	}
	fprintf(fd, "%d\n", sum);
	fclose(fd);
	return 0;
}
