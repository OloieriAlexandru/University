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
	pid_t pid;
	if (-1 == mkfifo("pipe1",0600))
	{
		if (errno == EEXIST)
		{
			printf("Canalul exista deja!\n");	
		}
		else
		{
			fprintf(stderr, "Eroare la crearea pipe-ului!\n");
			perror("Cauza este");
			exit(2);
		}
	}

	if (-1 == (pid = fork()))
	{
		perror("Eroare la fork()\n");
	}
	if (pid == 0)
	{
		// ma aflu in fiu
		int rd, fd = open("pipe1", O_RDONLY);
		char ch;
		while (1)
		{
			rd = read(fd, &ch, 1);
			if (rd == 0) break;
			if (rd == -1)
			{
				perror("Eroare la citirea din pipe");
				exit(3);
			}
			printf("%c",ch);
		}
		close(fd);
	}
	else
	{
		// ma aflu in parinte
		int fd = open("pipe1", O_WRONLY);
		char ch;

		while (scanf("%c",&ch) != EOF)
			if (ch >= 'a' && ch <= 'z')
				write(fd, &ch,1);

		close(fd);
		wait(NULL);
	}
	return 0;
}
