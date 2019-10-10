#include "Autoturism.h"

Autoturism::Autoturism (std::string m, std::string c)
{
	model = m;
	culoare = c;
}

void Autoturism::afiseaza ()
{
	std::cout << "Autoturism marca " << model << ", culoarea: " << culoare << '\n';
}
