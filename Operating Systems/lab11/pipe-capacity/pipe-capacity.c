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
	int p[2], dimens;
	char ch = '0';

	if (-1 == pipe(p))
	{
		perror("Eroare la crearea canalului anonim. Cauza erorii");
		exit(1);
	}
	if (-1 == fcntl(p[1], F_SETFL,O_NONBLOCK))
	{
		perror("Eroare la setarea non-blocking. Cauza erorii");
		exit(2);
	}
	dimens = 0;
	printf("Incep sa umplu canalul anonim...\n");

	while(1)
	{
		if (-1 == write(p[1],&ch,1))
		{
			perror("\nEroare (posibil umplere pipe?). Cauza erorii");
			fprintf(stderr, "errno = %d\n", errno);
			if (errno == EAGAIN)
			{
				fprintf(stderr, "Intr-adevar, cauza erorii este umplerea canalului!\n");
				break;
			}
			else
			{
				fprintf(stderr, "Cauza erorii este alta decat umplerea canalului!\n");
				exit(3);
			}
		}
		else
		{
			++dimens;
			if (dimens%1024 == 0)
			{
				printf(" %d Ko ... ", dimens/1024);
				fflush(stdout);
				usleep(50000);
			}
		}
		printf("Capacitatea unui canal anonim este %d octeti.\n",dimens);
	}

	return 0;
}
