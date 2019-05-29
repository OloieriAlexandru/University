#include <iostream>
#include "Mar.h"
#include "Portocala.h"
#include "Lamaie.h"
#include "Grepfruit.h"
#include "VitaminCVisitor.h"

int main ()
{
	VitaminCVisitor vitaminaC;
	Portocala p1(10, "portocaliu1");
	Portocala p2(20, "portocaliu2");
	std::cout<<"Portocala 1 are "<<p1.Accept (vitaminaC)<<" g de vitamina C\n";
	std::cout<<"Portocala 2 are "<<p2.Accept (vitaminaC)<<" g de vitamina C\n";
	Grepfruit g1 (100, "rosu1");
	std::cout << "Grepfruit ul 1 are " << g1.Accept (vitaminaC) << " g de vitamina C\n";
	Lamaie l1 (50, "galben1");
	std::cout << "Lamaia 1 are " << l1.Accept (vitaminaC) << " g de vitamina C\n";
	Mar m1 (80, "verde1");
	// m1.Accept (vitaminaC); // -> eroare
	return 0;
}