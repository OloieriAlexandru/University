#pragma once
#include <iostream>
#include <cstring>

class MyString
{
	char * sir;
	unsigned int AllocatedSize;
	unsigned int Size;

	void Resize (unsigned expectedLength);

public:
	MyString (); // aloca 16 octeti pentru sir-ul 
	MyString (const char * text); // aloca spatiu suficient pentru a copia si continutul lui text in sir
	~MyString (); // dealoca stringul

	unsigned int GetSize ();

	void Set (const char *text); // copie in sir pe text. Daca e nevoie, redimensioneaza pe sir
	void Set (MyString &m); // similar ca la functia Set de mai sus

	void Add (const char *text); // adauga la sfarsitul sirului curent pe text. Daca e nevoie redimensioneaza pe sir
	void Add (MyString &m); // similar ca Add de mai sus

	const char* GetText (); // returneaza textul

	// returneaza un obiect nou MyString in care copie din obiectul curent sub-sirul care incepe de la pozitia start si are lunginea size.
	// daca pozitia start e in afara sirului meu, sau dimensiunea ceruta (size) e prea mare, functia returneaza NULL
	MyString* SubString (unsigned int start, unsigned int size);


	// sterge caracterele din sir de la pozitia start, de dimensiunea size. Returneaza true daca parametri sunt corecti.
	// daca pozitia start e in afara sirului meu, sau dimensiunea ceruta (size) e prea mare, functia returneaza false 
	bool Delete (unsigned int start, unsigned int size);

	int Compare (const char * text); // compara sirul curent cu un alt text. Returneaza -1 daca sirul e mai mic decat text, 0 daca sunt egale si 1 altfel
	int Compare (MyString &m); // similar cu functia Compare de mai sus

	char GetChar (unsigned int index); // returneaza caracterrul de pe pozitia index. Daca pozitia e in afara sirului returneaza 0

	// insereaza textul text la pozitia index in sir. Daca index e in afara index-ului returneaza false; 
	// face si redimensionare daca e cazul
	bool Insert (unsigned int index, const char* text);

	bool Insert (unsigned int index, MyString &m); // similar cu functia Insert de mai sus.

	int Find (const char * text); // returneaza prima pozitie pe care a gasit sub-sirul text in 'sir'. Daca nu il gaseste returneaza -1

	int FindLast (const char * text); // returneaza ultima pozitie pe care a gasit sub-sirul text in 'sir'. Daca nu il gaseste returneaza -1

	friend std::ostream& operator <<(std::ostream &out, const MyString &str);
};