#include <iostream>
#include "Propozitie.h"

int main ()
{
	Propozitie p ("Ana are mere");
	std::cout << p[1] << std::endl;
	std::cout << std::boolalpha << p.Contains ("MeRE", true) << std::endl;
	std::cout << std::boolalpha << p.Contains ("ere", false) << std::endl;
	std::cout << (char*)p << std::endl;
	for (auto w : p)
	{
		std::cout << w << std::endl;
	}
	std::cout << p.GetNumberOfWords () << std::endl;
	return 0;
}