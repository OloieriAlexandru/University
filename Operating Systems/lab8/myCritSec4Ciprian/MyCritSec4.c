#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>

void modif_datafile(int fd1, int fd2);

int main(int argc,char *argv[])
{
	int fd1,fd2;
	if(argc < 3)
	{
		printf("Usage: %s datafile fisier cu comenzi\n",argv[0]); exit(1);
	}

	if(-1 == (fd1 = open(argv[1],O_RDWR)))
	{
		perror("Eroare la deschiderea fisierului de date");  exit(2);
	}

	if(-1 == (fd2 = open(argv[2],O_RDWR)))
	{
		perror("Eroare la deschiderea fisierului de date");  exit(2);
	}

	modif_datafile(fd1,fd2);

	close(fd1);
	close(fd2);
	fprintf(stderr, "Notificare: [PID: %d] Am terminat de parcurs toate perechile din fisierul %s si de facut operatiile corespunzatoare asupra fisierului %s!\n",getpid(), argv[2], argv[1]);
	return 0;
}

void modif_datafile(int fd1, int fd2)
{
	int cheie1,valoare,cheie2,alter,cheie_aux,val_aux;
	int rcod1,rcod2,rcod3,rcod4;
	struct flock lockInfo;
	struct flock unlockInfo;
	
	while(1)
	{
		if(-1 == (rcod1 = read(fd2,&cheie2,sizeof(int))))
		{
			perror("Eroare la citirea din fisierul de date");  exit(3);	
		}
		if(0 == rcod1) break;
		if(-1 == (rcod2 = read(fd2,&alter,sizeof(int))))
		{
			perror("Eroare la citirea din fisierul de date");  exit(3);	
		}

		int gasit=0;
		int cnt=0;
		while(1)
		{
			if(-1 == (rcod3 = read(fd1,&cheie1,sizeof(int))))
			{
				perror("Eroare la citirea din fisierul de date");  exit(3);	
			}
			if(0 == rcod3) break;
			if(-1 == (rcod4 = read(fd1,&valoare,sizeof(int))))
			{
				perror("Eroare la citirea din fisierul de date");  exit(3);	
			}

			if(cheie1 == cheie2)
			{
				gasit = 1;
				break;
			}
			cnt++;
		}
		
		if(gasit == 1)
		{
			lockInfo.l_type=F_WRLCK;
        		lockInfo.l_whence=SEEK_SET;
        		lockInfo.l_start=2*cnt*sizeof(int);
        		lockInfo.l_len=2*sizeof(int);

			/* O singura incercare de punere a lacatului in mod blocant */
			printf("Pun blocant lacatul pe cheie si valoarea ei [Proces:%d].\n",getpid());
			if(-1 == fcntl(fd1,F_SETLKW,&lockInfo) )
			{ 
				fprintf(stderr,"Eroare la blocaj [ProcesID:%d].\n", getpid());
				perror("\tMotivul");
				exit(4);
			}
			else
				printf("Blocaj reusit [ProcesID:%d].\n", getpid());
			sleep(1);
			
			if(-1 == lseek(fd1, 2*cnt*sizeof(int), SEEK_SET)) { perror("Eroare la apelul lseek"); exit(5); }

			if ((-1 == read(fd1, &cheie_aux, sizeof(int))) && (-1 == read(fd1, &val_aux, sizeof(int))))
			{
				perror("Eroare la apelul read");
				exit(6);
			}

			if(valoare != val_aux)
			{
				printf("Valoarea corespunzatoare cheii s-a modificat intre timp. Reactualizam.\n");
				valoare=val_aux;
			}
			valoare = valoare + alter;

			if(-1 == lseek(fd1, (2*cnt+1)*sizeof(int), SEEK_SET)) { perror("Eroare la apelul lseek"); exit(5); }

			if(-1 == write(fd1, &valoare, sizeof(int)))
			{
            			perror("Eroare la scrierea valorii actualizate in fisierul de date...");
				exit(7);
			}

			unlockInfo.l_type=F_UNLCK;
        		unlockInfo.l_whence=SEEK_SET;
			unlockInfo.l_start=2*cnt*sizeof(int);
			unlockInfo.l_len=2*sizeof(int);
			if(-1 == fcntl(fd1,F_SETLKW, &unlockInfo))
			{
				perror("Eroare la deblocaj");
				exit(8);
			}

			sleep( random() % 5 );
		}
		else
		{
			lockInfo.l_type=F_WRLCK;
        		lockInfo.l_whence=SEEK_END;
        		lockInfo.l_start=0;
        		lockInfo.l_len=2*sizeof(int);

			/* O singura incercare de punere a lacatului in mod blocant */
			printf("Pun blocant lacatul pe finalul fisierului [Proces:%d].\n",getpid());
			if(-1 == fcntl(fd1,F_SETLKW,&lockInfo) )
			{ 
				fprintf(stderr,"Eroare la blocaj [ProcesID:%d].\n", getpid());
				perror("\tMotivul");
				exit(4);
			}
			else
				printf("Blocaj reusit [ProcesID:%d].\n", getpid());
			sleep(1);

			if(-1 == lseek(fd1, 0, SEEK_CUR)) { perror("Eroare la apelul lseek"); exit(5); }

			if (0 == read(fd1, &cheie_aux, sizeof(int)))
			{
				printf("Cheia a ramas neadaugata. Adaugam.\n");
				if(-1 == lseek(fd1, 0, SEEK_CUR)) { perror("Eroare la apelul lseek"); exit(5); }
				if((-1 == write(fd1,&cheie2,sizeof(int))) || (-1 == write(fd1,&alter,sizeof(int))))
				{
					perror("Eroare la scrierea in fisierul de date");  exit(7);
				}

				unlockInfo.l_type=F_UNLCK;
        			unlockInfo.l_whence=SEEK_END;
				unlockInfo.l_start=-2*sizeof(int);
				unlockInfo.l_len=2*sizeof(int);
				if(-1 == fcntl(fd1,F_SETLKW, &unlockInfo))
				{
					perror("Eroare la deblocaj");
					exit(8);
				}

				sleep( random() % 5 );
			}
			else
			{
				printf("Cheia a fost adaugata intre timp. Reactualizam valoarea corespunzatoare ei.\n");
				if(-1 == read(fd1, &val_aux, sizeof(int)))
				{
					perror("Eroare la apelul read");
					exit(6);
				}
				valoare = val_aux;
				valoare = valoare + alter;

				if(-1 == lseek(fd1, -sizeof(int), SEEK_END)) { perror("Eroare la apelul lseek"); exit(5); }

				if(-1 == write(fd1, &valoare, sizeof(int)))
				{
            				perror("Eroare la scrierea valorii actualizate in fisierul de date...");
					exit(7);
				}

				unlockInfo.l_type=F_UNLCK;
        			unlockInfo.l_whence=SEEK_END;
				unlockInfo.l_start=-2*sizeof(int);
				unlockInfo.l_len=2*sizeof(int);
				if(-1 == fcntl(fd1,F_SETLKW, &unlockInfo))
				{
					perror("Eroare la deblocaj");
					exit(8);
				}

				sleep( random() % 5 );
			}
		}
	}
}
