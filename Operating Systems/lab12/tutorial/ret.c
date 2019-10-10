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
	int fdR, fdW, fdRW, codW, codR;
	char bufW[10] = "123abc789";
	char bufR[10] ;
	char *canal;

	if (argc != 2)
	{
		fprintf(stderr, "Sintaxa apel: ret nume_fifo\n");
		exit(1);
	}

	canal = argv[1];

	char comanda_ls[100] = "ls -l ";
	strcat(comanda_ls, canal);

	if (-1 == mkfifo(canal, 0600))
	{
		if (errno == EEXIST)
		{
			fprintf(stdout, "Observatie: fisierul fifo %s exista deja !\n", canal);
		}
		else
		{
			fprintf(stderr, "Eroare: creare fisier fifo imposibila, errno=%d\n", errno);
			perror("Cauza erorii este");
			exit(2);
		}
	}

	fdR = open(canal, O_RDONLY | O_NONBLOCK);
	if (-1 == fdR)
	{
		perror("Eroare la deschiderea capatului de citire al canalului");
		exit(3);
	}

	fdW = open(canal, O_WRONLY);
	if (-1 == fdW)
	{
		perror("Eroare la deschiderea capatului de scriere al canalului");
		exit(4);
	}

	codW = write(fdW, bufW, strlen(bufW));
	if (-1 == codW)
	{
		perror("Eroare la scrierea unui mesaj in canal");
		exit(5);
	}
	else
		printf("\nS-au scris %d octeti in canal -> si anume, secventa de caractere: '%s'\n", codW, bufW);

	sleep(1);
	printf("Dimensiunea canalului dupa scrierea in el:\n");
	system(comanda_ls);

	sleep(5);
	codR = read(fdR,bufR,3);
	if (-1 == codR)
	{
		perror("Eroare la prima citire din canal");
		if (errno == EAGAIN)
		{
			fprintf(stderr, "Eroare EAAIN la citire (i.e. canalul este gol)!\n");
		}
		else
		{
			fprintf(stderr, "Eroare errno=%d la citire.\n", errno);
		}
		exit(6);
	}
	bufR[codR] = 0;
	printf("Dimensiunea canalului dupa prima citire a 3 octeti din el:\n");
	system(comanda_ls);

	sleep(2);
	close(fdW);
	printf("\nAm inchis capatul de scriere. Dimensiunea canalului dupa inchiderea capatului de scriere:\n");
	system(comanda_ls);

	sleep(5);
	codR = read(fdR, bufR, 3);
	if (-1 == codR)
	{
		perror("Eroare la a doua citire din canal");
		if (errno == EAGAIN)
		{
			fprintf(stderr, "Eroare EAGAIN la citire (i.e. canalul este gol)!\n");
			printf("Asadar, nu s-a mai putut citi nimic din canal dupa inchiderea capatului de scriere (i.e., ceilalti 6 octeti din canal s-au pierdut).\n");
			exit(7);
		}
		else
		{
			fprintf(stderr, "Eroare errno=%d la citire.\n", errno);
			exit(8);
		}
	}
	else
	{
		bufR[codR] = 0;
		printf("\nS-au citit din canal urmatorii %d octeti: %s\n", codR, bufR);
	}

	sleep(2);
	close(fdR);
	printf("\nAm inchis si capatul de citire. Dimensiunea canalului in acest moment:\n");
	system(comanda_ls);

	sleep(5);
	fdRW = open(canal, O_RDWR | O_NONBLOCK);
	if (-1 == fdRW)
	{
		perror("Eroare la redeschiderea canalului (simulan la ambele capete ale canalului)");
		exit(9);
	}

	codR = read(fdRW,bufR,3);
	if (-1 == codR)
	{
		perror("\nEroare la a treia citire din canal");
		if (errno == EAGAIN)
		{
			fprintf(stderr, "Eroare EAGAIN la citire (i.e. canalul este gol)!\n");
			printf("Asadar, nu s-a mai putut citi nimic din canal dupa inchiderea ambelor capete si apoi redeschiderea lor\n");
			exit(10);
		}
		else
		{
			fprintf(stderr, "Eroare errno=%d la citire.\n", errno);
			exit(11);
		}
	}
	else
	{
		bufR[codR] = 0;
		printf("\nS-au citit din canal ultimii %d octeti: %s\n", codR, bufR);
	}
	close(fdRW);

	sleep(1);
	printf("\nSfarsitul programului de testare a retentiei.\n");
	return 0;
}
