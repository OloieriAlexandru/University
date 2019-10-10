#include "Transport.h"

Transport::Transport (std::string m, std::string c)
{
	model = m;
	culoare = c;
}

void Transport::Adauga (Autovehicul * a)
{
	autos.push_back (a);
}

void Transport::afiseaza ()
{
	std::cout << "Transportor marca " << model << ", culoarea: " << culoare << "\n";
	std::cout << "Transport:\n";
	for (int i = 0; i < (int)autos.size (); ++i)
	{
		std::cout << "    " << i << ". ";
		autos[i]->afiseaza ();
	}
}
