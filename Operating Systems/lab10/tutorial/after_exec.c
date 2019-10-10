// after_exec.c
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
	int nrBytesRead;
	char ch;

	printf("Caracteristici dupa exec\n");
	printf("-------------------------\n");
	printf("ID-ul procesului: %d\n",getpid());
	printf("ID-ul parintelui: %d\n",getppid());
	printf("Proprietarul real: %d\n", getuid());
	printf("Proprietarul efectiv: %d\n", geteuid());
	printf("Directorul curent de lucru: %s\n\n", getcwd(tab_ref, 1000));

	
  	nrBytesRead = read(STDIN_FILENO, &ch, 1);
  	printf("Numarul de caractere citite: %d\n",nrBytesRead);
  	if( -1 == nrBytesRead )
    		perror("Eroare la citirea din fluxul stdin (deoarece este inchisa !) ");
	return 0;
}
