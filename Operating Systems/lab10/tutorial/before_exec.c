// before_exec.c
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

char tab_ref[1000];

int main(int argc, char *argv[])
{	
	printf("Caracteristici inainte de exec\n");
  	printf("------------------------------\n");
  	printf("ID-ul procesului : %d\n",getpid());
  	printf("ID-ul parintelui : %d\n",getppid());
  	printf("Proprietarul real : %d\n",getuid());
  	printf("Proprietarul efectiv : %d\n",geteuid());
  	printf("Directorul curent de lucru : %s\n\n",getcwd(tab_ref,1000));

  	/* cerere de inchidere a intrarii standard la reacoperire */
  	fcntl(STDIN_FILENO, F_SETFD, FD_CLOEXEC);

  	/* reacoperire */
  	execl("afterexe","after_exec",NULL);

  	/* se executa numai daca esueaza apelul exec */
 	 perror("Eroare la apelul execl");
  	return 1;
}
