#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>

void initializare_fisier_date();
/*
*  Pentru început, avem nevoie de o rutină ajutătoare, care să creeze conținutul inițial al fișierului de date partajat
*  Se va crea fișierul și va conține un număr real, stocat în format binar, reprezentând cantitatea inițială de combustibil din rezervor.
*  Vom apela programul cu opțiunea -i pentru a invoca această rutină de inițializare.
*/

void afisare_fisier_date();
/*
*  Similar, la finalul tuturor actualizărilor, vom avea nevoie de o altă rutină ajutătoare, care să ne afișeze conținutul final din rezervor.
*  Vom apela programul cu opțiunea -o pentru a invoca această rutină de afișare finală.
*/

void secventa_de_actualizari(int n, char* nr[]);
/*
   Rutina principală a programului, care va executa secvența de actualizări primită în linia de comandă, în manieră concurentă, conform celor specificate în enunțul problemei.
*/

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        fprintf(stderr, "Eroare: programul trebuie apelat cu optiunile: -i, -o, sau cu o secventa de numere reale...\n");  exit(1);
    }

    /* cazul de intializare a fisierului de date */
    if( strcmp(argv[1],"-i") == 0 )
    {
        initializare_fisier_date();  return 0;
    }

    /* cazul de afisare a fisierului de date */
    if( strcmp(argv[1],"-o") == 0 )
    {
        afisare_fisier_date();  return 0;
    }

    /* cazul general: actualizarea fisierului de date */
    secventa_de_actualizari(argc, argv);
    return 0;
}


void initializare_fisier_date()
{/* Functionalitate: intializarea fisierului de date */

    int fd;
    float val;

    printf("Dati cantitatea initiala de combustibil (i.e., un numar real pozitiv): ");
    while( (1 != scanf("%f", &val)) && (val < 0) ) { fprintf(stderr,"Eroare: nu ati introdus un numar real pozitiv! Incercati din nou..."); }

    if(-1 == (fd = open("peco.bin", O_WRONLY | O_CREAT | O_TRUNC, 0600) ) )
    {
        perror("Eroare la crearea fisierului de date...");  exit(2);
    }

    if(-1 == write(fd, &val, sizeof(float) ) )
    {
        perror("Eroare la scrierea valorii initiale in fisierul de date...");  exit(3);
    }

    close(fd);
}


void afisare_fisier_date()
{/* Functionalitate: afisarea fisierului de date */

    int fd;
    float val;

    if ( -1 == (fd = open("peco.bin", O_RDONLY) ) )
    {
        perror("Eroare la deschiderea pentru afisare a fisierului de date...");  exit(4);
    }

    if (-1 == read(fd, &val, sizeof(float) ) )
    {
        perror("Eroare la citirea valorii finale din fisierul de date...");  exit(5);
    }

    close(fd);
    printf("Stocul final de combustibil este: %f litri de combustibil.\n", val);
}


void secventa_de_actualizari(int n, char* nr[])
{ /* Functionalitate: secventa de operatii de actualizare a fisierului de date */

    float valoare, stoc, stoc_nou;
    int fd,i;

    if ( -1 == (fd = open("peco.bin", O_RDWR) ) )
    {
        perror("Eroare la deschiderea pentru actualizare a fisierului de date...");  exit(6);
    }

    srandom( getpid() ); // initializarea generatorului de numere aleatoare

    for(i = 1; i < n; i++) // iteram prin secventa de numere reale specificata in linia de comanda
    {
        if(1 != sscanf(nr[i], "%f", &valoare))  // facem conversie de la string la float
        {
            fprintf(stderr, "[PID: %d] Eroare: ati specificat o valoare invalida drept argument de tip numar real : %s !\n", getpid(), nr[i] );
            exit(7);
        }

        /* Inceput sectiune critica */
        struct flock lockInfo;
        lockInfo.l_type=F_WRLCK;
        lockInfo.l_whence=SEEK_SET;
        lockInfo.l_start=0;
        lockInfo.l_len=sizeof(float);

        if(-1 == lseek(fd, 0, SEEK_SET) ) // (re)pozitionam cursorul la inceputul fisierului
        {
            perror("Eroare la repozitionarea in fisierul de date, pentru citire...");  exit(8);
        }
        if (-1 == read(fd, &stoc, sizeof(float) ) )
        {
            perror("Eroare la citirea valorii din fisierul de date...");  exit(9);
        }

	 /* O singura incercare de punere a lacatului in mod blocant */
        printf("Pun blocant lacatul pe valoarea stocului [Proces:%d].\n",getpid());
        if(-1 == fcntl(fd,F_SETLKW,&lockInfo) )
        { 
          fprintf(stderr,"Eroare la blocaj [ProcesID:%d].\n", getpid());
          perror("\tMotivul");
          exit(10);
        }
        else
          printf("Blocaj reusit [ProcesID:%d].\n", getpid());

	sleep(2);

	if(-1 == lseek(fd, 0, SEEK_SET) ) { perror("Eroare la apelul lseek"); exit(8); }

	if (-1 == read(fd, &stoc_nou, sizeof(float) ) ) { perror("Eroare la apelul read"); exit(9); }

	if(stoc != stoc_nou)
	{
	    printf("Stocul s-a modificat intre timp. Reactualizam.\n");
	    stoc=stoc_nou;
	}

        printf("[PID: %d] Se adauga/extrage in/din rezervor cantitatea de %f litri de combustibil.\n", getpid(), valoare );

        stoc = stoc + valoare;
        if(stoc < 0)
        {
            printf("Rezervorul nu are destul combustibil! Doriti sa extrageti doar cat este disponibil?\n");
            char *answer;
	    answer = (char *) malloc(2*sizeof(char));
	    memset(answer, 0, sizeof(answer));
	    printf("Urmeaza sa citesc!\n");
            long unsigned int len = 2; 
	    int read = getline(&answer, &len, stdin);
	    if (read == -1)
	    {
		printf("Nu ai introdus un raspuns corect\n");
	    }
	    else
	    {
		printf("%s\n", answer);
	    }
	    
	    printf("Merge citirea!%d\n",(int)strlen(answer));
	    int j;
            for(j=0;answer[j]!=0;j++)
            {
                answer[j]=toupper(answer[j]);
            }
            if(strcmp(answer,"DA") == 0)
            {
                stoc=stoc-valoare;
                printf("Ati acceptat sa extrageti %f. Stocul este acum 0!",stoc);
                stoc=0;
            }
            else
            {
                stoc=stoc-valoare;
                printf("Ati refuzat sa extrageti cat a fost disponibil. Stocul a ramas %f.",stoc);
            }
        }
	
        if(-1 == lseek(fd, 0, SEEK_SET) ) // repozitionam cursorul la inceputul fisierului
        {
            perror("Eroare la repozitionarea in fisierul de date, pentru scriere...");  exit(11);
        }
        if (-1 == write(fd, &stoc, sizeof(float) ) )
        {
            perror("Eroare la scrierea valorii actualizate in fisierul de date..."); exit(12);
        }

        /* Sfarsit sectiune critica */
        struct flock unlockInfo;
        unlockInfo.l_type=F_UNLCK;
        unlockInfo.l_whence=SEEK_SET;
        unlockInfo.l_start=0;
        unlockInfo.l_len=sizeof(float);
        if(-1 == fcntl(fd,F_SETLKW, &unlockInfo))
	{
	    perror("Eroare la deblocaj");  exit(13);
	}

        sleep( random() % 5 ); // o pauza aleatoare, de maxim 4 secunde, inainte de a continua cu urmatoarea iteratie
    }

    close(fd);
    printf("[PID: %d] Am terminat de procesat secventa de actualizari.\n", getpid());
}



/*   *** Ghid de utilizare ***
Pentru execuție, se va crea mai întâi fișierul partajat, i.e. executând comanda:
UNIX> ./prg_sc1 -i

Apoi se vor rula în paralel mai multe instanțe ale programului, fiecare cu propria secvență de actualizări.
Cu alte cuvinte, lansarea simultană în execuție concurentă a mai multor instanțe ale programului se va face astfel:

UNIX> ./prg_sc1 25500 -10,9 -35,42 3500 -20,45 ... &  ./prg_sc1 -10,5 -34,2 2500 -25,56 ... &  ./prg_sc1 ... & ...

Iar la final veți dori să aflați cantitatea rămasă în rezervor după seria de actualizări de mai sus, i.e. rulați comanda:
UNIX> ./prg_sc1 -o
*/
