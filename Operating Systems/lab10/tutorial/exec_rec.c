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

/* extern int errno; // variabila declarata in errno.h */

int main(int argc, char* argv[], char* env[])
{
  char **r, *s, *w[5];
  printf("PID=%d, PPID=%d, OWNER=%d\n",getpid(),getppid(),getuid());
  printf("ENVIRONMENT:\n");
  r=env;
  while(1)
  { 
     s=*r++;
     if(s == NULL) break;
     printf("%s\n",s);
  }
  putchar('\n');

  env[0]="Salut.";
  env[1]=NULL;

  w[0]=argv[0]; /* numele executabilului ! */
  w[1]="2nd call";
  w[2]=NULL;

  if( (argv[1] != NULL) && (argv[1][0] == '2') )
  {
     exit(0); /* oprire recursie la al doilea apel ! */
  }
  else
  {
     printf("Urmeaza apelul primitivei exec.\n");
     if( execve(argv[0], w, env) == -1)
     {
        perror("Eroare la apelul execve");
        exit(1);
     }
  }
  return 0;
}
