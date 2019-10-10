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

int main(void)
{
	int fd,dimens;
	char ch='0';

	/* creare canal fifo */
	if(-1 == mkfifo("canal-extern", 0600) )
	{
		if(errno == EEXIST)   // errno=17 for "File already exists"
			fprintf(stdout,"Nota: canalul fifo 'canal-extern' exista deja !\n");
		else
		{
			perror("Eroare la crearea canalului fifo. Cauza erorii");  exit(1);
		}
	}

	/* deschidere cu setare non-blocking necesara pentru capatul de scriere */
	fd = open("canal-extern",O_RDWR | O_NONBLOCK);
	if(-1 == fd)
	{
		perror("Eroare la deschiderea fisierului fifo cu setare non-blocking. Cauza erorii");  exit(2);
	}

	dimens=0;
	printf("Incep sa umplu canalul fifo ...\n");

	while(1)
	{
		if(-1 == write(fd,&ch,1) )
		{
			perror("\nEroare (posibil umplere pipe ?). Cauza erorii");
			fprintf(stderr,"errno = %d\n", errno);
			if(errno == EAGAIN)
			{
				fprintf(stderr,"Intr-adevar, cauza erorii este umplerea canalului!\n");
				break;
			}
			else
			{
				fprintf(stderr,"Cauza erorii este alta decat umplerea canalului!\n");
				exit(3);
			}
		}
		else
		{
			++dimens;
			if(dimens%1024 == 0) { printf(" %d Ko ...  ", dimens/1024);  fflush(stdout);  usleep(50000); } // mesaje interactive pe ecran...
		}
	}

	printf("Capacitatea unui canal fifo este de %d octeti.\n", dimens);
	return 0;
}
