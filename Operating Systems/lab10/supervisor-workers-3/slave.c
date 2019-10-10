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
	int nr1, nr2, ans;
	char op;
	if (argc < 4)
	{
		fprintf(stderr, "Programul trebuie apelat cu minim 3 argumente: numar1 operatie numar2\n");
		return 255;
	}
	if ((1 != sscanf(argv[1],"%d",&nr1)) || (1 != sscanf(argv[3],"%d",&nr2)))
	{
		fprintf(stderr, "Argumente invalide\n");
		return 255;
	}
	if (1 != sscanf(argv[2],"%c",&op) ||(op!='+' && op!='-'))
	{
		fprintf(stderr, "Operatia trebuie sa fie un caracter, + sau -\n");
		return 255;
	}
	if (op == '+')
		ans = nr1 + nr2;
	else
		ans = nr1 - nr2;
	exit(ans);
}
