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
	int fd;

  /* redirectam fluxul stdout in fisierul fis.txt */
  	fd=creat("fis.txt",0600);
  	close(1);  /* inchid stdout */
  	dup(fd);   /* duplic fd cu 1 (primul gasit liber) */
  	close(fd); /* inchid fd */

  /* scriem pe stdout */
  	write(1,"Salut",5);
  	execl("com-2","com-2",NULL);


  /* se executa numai daca esueaza apelul exec */
  	perror("Eroare la apelul execl");
  	return 1;
}
