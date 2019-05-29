#include <iostream>
#include "Contact.h"
#include "Prieten.h"
#include "Agenda.h"
#include "Coleg.h"
#include "Cunoscut.h"

Agenda ag;

int main ()
{
	Prieten p1("Gigel", "12-12-2012", "0757555777", "strada 1");
	ag.Add (&p1);
	Coleg c1 ("Persoana 3","0746372829","Bit", "a doua strada 2");
	ag.Add (&c1);
	Cunoscut ct1 ("Persoana 2", "0756748392");
	ag.Add (&ct1);
	Prieten p2 ("Persoana 1", "06-06-2006", "0756740192", "a treia strada 3");
	ag.Add (&p2);
	ag.PrintAll ();
	auto fr = ag.GetFriends ();
	std::cout << '\n';
	for (auto x : fr)
		std::cout << x.GetName () << '\n';
	p1.SetName ("GigelSchimbat");
	ag.PrintAll ();
	p1.SetName ("Gigel");
	std::cout << '\n';
	if (ag.Find ("Gigel"))
		std::cout << "Gigel este in agenda\n";
	else
		std::cout << "Gigel nu este in agenda\n";
	ag.Delete ("Gigel");
	if (ag.Find ("Gigel"))
		std::cout << "Gigel este in agenda\n";
	else
		std::cout << "Gigel nu este in agenda\n";

	return 0;
}