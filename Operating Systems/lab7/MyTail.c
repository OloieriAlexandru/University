#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>

int printLastLines = 1;
int numberToPrint = 10;
int minusN, minusC;
int minusNIndex = -1, minusCIndex = -1;
int numberNIndex = -1, numberCIndex = -1;
int numberOfProcessedFiles;

void processMinusN(char *fileName)
{
	FILE *fin=fopen(fileName,"r");
	if (fin == NULL)
	{
		printf("Eroare la deschidere\n");
		return;
	}
	int fileNumberOfLines = 0;
	char ch;
	while (1)
	{
		ch = fgetc(fin);
		if (feof(fin)) break;
		if (ch == '\n') ++fileNumberOfLines;
	}
	if (ch == '\n') ++fileNumberOfLines;
	fclose(fin);
	
	int currLine = 0;
	int startWith = fileNumberOfLines - numberToPrint;
	if (startWith < 0) startWith = 0;
	fin=fopen(fileName,"r");
	while (1)
	{
		ch = fgetc(fin);
		if (feof(fin)) break;
		if (currLine >= startWith)
			printf("%c", ch);
		if (ch == '\n') ++currLine;
	}
	fclose(fin);
}

void processMinusC(char *fileName)
{
	FILE *fin=fopen(fileName,"r");
	if (fin == NULL)
	{
		printf("Eroare la deschidere\n");
		return;
	}
	int fileNumberOfCharacters = 0;
	char ch;
	while (1)
	{
		ch = fgetc(fin);
		if (feof(fin)) break;
		++fileNumberOfCharacters;
	}
	++ch;
	fclose(fin);
	
	int currChar = 0;
	int startWith = fileNumberOfCharacters - numberToPrint;
	if (startWith < 0) startWith = 0;
	fin=fopen(fileName,"r");
	while (1)
	{
		ch = fgetc(fin);
		if (feof(fin)) break;
		if (currChar >= startWith)
			printf("%c", ch);
		++currChar;
	}
	fclose(fin);
}

void processFile(char *fileName)
{
	printf("Fisierul %s:\n", fileName);
	if (printLastLines)
		processMinusN(fileName);
	else
		processMinusC(fileName);
	printf("\n");
}

int isNumber(char *str)
{
	if (!(*str)) return 0;
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9')) return 0;
		++str;
	}	
	return 1;
}

int getNumber(char *str)
{
	int ans = 0;
	while (*str)
	{
		ans = ans * 10 + (*str - '0');
		++str;
	}
	return ans;
}

int startsWith(char *str, char *pattern)
{
	while (*str && *pattern)
	{
		if (*str != *pattern) return 0;
		++str, ++pattern;
	}
	if (!(*pattern)) return 1;
	return 0;
}

int main(int argc, char *argv[])
{	
	if (argc == 1)
	{
		printf("Trebuie sa specifici cel putin un fisier\n");
		return 0;
	}
	int i;
	for (i=1;i<argc;++i)
	{
		if (startsWith(argv[i], "-n"))
		{
			minusN = 1;
			minusNIndex = i;
			if (isNumber(argv[i] + 2))
			{
				numberToPrint = getNumber(argv[i] + 2);
			}	
			else
			{
				continue;
				if (i<argc-1)
				{
					if (isNumber(argv[i+1]))
						numberToPrint = getNumber(argv[i+1]), numberNIndex = i+1;
					else
					{
						printf("Trebuie sa specifici un numar pentru \"-n\"\n");
						return 0;
					}
				}
				else
				{
					printf("Trebuie sa specifici un numar pentru \"-n\"\n");
					return 0;
				}
			}
		} else
		if (startsWith(argv[i], "-c"))
		{
			minusC = 1;
			printLastLines = 0;
			minusCIndex = i;
			if (isNumber(argv[i] + 2))
			{
				numberToPrint = getNumber(argv[i] + 2);
			}	
			else
			{
				continue;
				if (i<argc-1)
				{
					if (isNumber(argv[i+1]))
						numberToPrint = getNumber(argv[i+1]), numberCIndex = i+1;
					else
					{
						printf("Trebuie sa specifici un numar pentru \"-c\"\n");
						return 0;
					}
				}
				else
				{
					printf("Trebuie sa specifici un numar pentru \"-c\"\n");
					return 0;
				}
			}
		}
	}
	if (minusC && minusN)
	{
		printf("Trebuie sa specifici cel mult una dintre urmatoarele optiuni: \"-n\" sau \"-c\"");
		return 0;
	}
	for (int i=1;i<argc;++i)
	{
		if (i == numberCIndex || i == numberNIndex || i == minusCIndex || i == minusNIndex) continue;
		processFile(argv[i]);
	}
	return 0;
}
