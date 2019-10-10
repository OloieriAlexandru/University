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
	int sum = 0, codr, nr;
	while (1)
	{
		codr = read(0,&nr,sizeof(nr));
		if (codr == -1)
		{
			perror("Eroare la citirea din stdin intr-unul din slave");
			exit(1);
		}
		if (!codr || !nr) break;
		sum+=nr;
	}
	write(1,&sum,sizeof(sum));
	close(1);
	return 0;
}
