/*#include <iostream>
#include "list.h"
#include "myObject.h"

List<int>listaInt;
List<myObject>listaMyObjects;

int main ()
{
	std::cout << "Valorile 12, 5, 24, 19, -1, 412 vor fi introduse in lista\n";
	int x = 12;
	listaInt.Add (x);
	x = 5;
	listaInt.Add (x);
	x = 24;
	listaInt.Add (x);
	x = 19;
	listaInt.Add (x);
	x = -1;
	listaInt.Add (x);
	x = 412;
	listaInt.Add (x);

	std::cout << "Valorile au fost adaugate in lista\n";

	listaInt.Print (std::cout);

	listaInt.Sort ();

	std::cout << "Valorile sortate:\n";

	listaInt.Print (std::cout);

	std::cout << "-----------------------------------------------------------\n";

	std::cout << "MyObject memoreaza perechi (x,y), mai exact, niste puncte in plan\n";
	std::cout << "Punctele (1,2), (4,5), (124,241), (-1, 0) vor fi introduse in lista\n";

	myObject obj (1, 2);
	listaMyObjects.Add (obj);
	obj.set (4, 5);
	listaMyObjects.Add (obj);
	obj.set (124, 241);
	listaMyObjects.Add (obj);
	obj.set (-1, 0);
	listaMyObjects.Add (obj);

	std::cout << "Punctele au fost introduse in lista:\n";
	
	listaMyObjects.Print (std::cout);

	listaMyObjects.Sort ();

	std::cout << "Punctele au fost sortate in functie de distanta fata de origine:\n";

	listaMyObjects.Print (std::cout);

	system ("pause");
	return 0;
}*/
#include <iostream>
#define INIT(x) x(x)
#define ABC class
#define AAA public
#define AFISEAZA(x) std::cout<<(x)<<' ';
#define X_DIN_T t.Get_x()
#define Y_DIN_T t.Get_y()
#define CAT_TIMP(x) while(x) {
#define DACA(x, y, z) if(x) {y;} else {z;}
#define MAI_MARE_DECAT >
#define MINUS -
#define ESTE_DIFERIT_DE !=
#define SFARSIT_CAT_TIMP }

#define CREATE(var)\
	void Set_##var(int v) {var = v;}\
	int Get_##var() { return var; }

ABC Test
{
int x,y,z;
AAA:
 Test () : INIT (x), INIT (y), INIT (z) {}
 CREATE (x);
 CREATE (y);
 CREATE (z);
};

int main ()
{
	Test t;
	t.Set_x (18);
	t.Set_y (24);
	AFISEAZA (t.Get_x () + t.Get_y ());
	CAT_TIMP (X_DIN_T ESTE_DIFERIT_DE Y_DIN_T)
	DACA (X_DIN_T MAI_MARE_DECAT Y_DIN_T, t.Set_x (X_DIN_T MINUS Y_DIN_T), t.Set_y (Y_DIN_T MINUS X_DIN_T));
	SFARSIT_CAT_TIMP
	AFISEAZA (X_DIN_T);

	system ("pause");
	return 0;
}
