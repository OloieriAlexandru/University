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
	printf("Salut");
	fflush(stdout); /* decomentati aceasta linie pentru a obtine mesajul intreg */
	execl("com-2","com-2",NULL);
	/* se executa numai daca esueaza apelul exec */	
	perror("Eroare la apelul execl");
	return 0;
}
