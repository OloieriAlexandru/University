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

#define nmax 1000

int main(int argc, char *argv[])
{	
	int pid, p[2];
	char ch;
	if (-1 == pipe(p))
	{
		perror("Eroare la crearea canalului intern!");
		exit(1);
	}

	if (-1 == (pid=fork()))
	{
		perror("Eroare la crearea unui proces fiu!");
		exit(2);
	}
	if (pid)
	{
		close(p[0]);
		printf("Introduceti o secventa de caractere diverse, terminata prin CTRL+D.\n");
		while (EOF != (ch = getchar()))
			if (ch>='a' && ch<='z')
			    write(p[1],&ch,1);
		close(p[1]);
		wait(NULL);
	}
	else
	{
		char buffer[nmax];
		int nIndex = 0;
		close(p[1]);
		while (read(p[0],&ch,1) != 0)
			if (nIndex < nmax)
				buffer[nIndex++] = ch;
		buffer [ (nIndex == nmax) ? nmax-1 : nIndex ] = 0;
		printf("[Procesul consumator (fiul)] Am citit din canal secventa: %s\n", buffer);
		close(p[0]);
	}
	return 0;
}
