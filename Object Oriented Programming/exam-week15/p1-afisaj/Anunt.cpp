#include "Anunt.h"

Anunt::Anunt (std::string msg)
{
	mesaj = msg;
}

void Anunt::Display ()
{
	std::cout << "ANUNT: [" << mesaj << "]\n";
}
