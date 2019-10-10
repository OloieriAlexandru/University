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
	int pid, p[2];
	char ch = 'A';

	if (-1 == pipe(p))
	{
		perror("Eroare la crearea canalului intern!");
		exit(1);
	}

	if (-1 == (pid = fork()))
	{
		perror("Eroare la fork!");
		exit(2);
	}
	if (pid)
	{
		printf("Incep!\n");
		close(p[0]);
		fcntl(p[1],F_SETFL,O_NONBLOCK);
		int nr = 0;
		while (1)
		{
			++nr;
			if (write(p[1],&ch,1)==-1)
			{
				break;
			}
		}
		--nr;
		printf("Canalul de comunicatie are %dKB\n",nr);
		close(p[1]);
		
		char processId[10];
		memset(processId, 0, sizeof(processId));
		sprintf(processId,"%d",pid);

		execlp("kill","kill","-9",processId,NULL);

		wait(NULL);
	}
	else
	{
		int s = 1;
		close(p[1]);
		while(1)
		{
			s = 1 - s;
		}

		close(p[0]);
	}

	return 0;
}
