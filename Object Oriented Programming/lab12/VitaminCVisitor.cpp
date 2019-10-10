#include "VitaminCVisitor.h"

VitaminCVisitor::VitaminCVisitor ()
{
}

double VitaminCVisitor::VisitLamaie (Lamaie * l)
{
	std::cout << "Am vizitat lamaia de culoare " << l->getCul() << " si dimensiune " << l->getDim() << '\n';
	return l->getDim() * 0.23;
}

double VitaminCVisitor::VisitPortocala (Portocala * p)
{
	std::cout << "Am vizitat portocala de culoare " << p->getCul() << " si dimensiune " << p->getDim() << '\n';
	return p->getDim() * 0.13;
}

double VitaminCVisitor::VisitGrepfruit (Grepfruit * g)
{
	std::cout << "Am vizitat grepfruit ul de culoare " << g->getCul() << " si dimensiune " << g->getDim() << '\n';
	return g->getDim() * 0.31;
}
