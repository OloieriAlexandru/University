#include "Portocala.h"

Portocala::Portocala (int dim, std::string cul)
{
	dimensiune = dim;
	culoare = cul;
}

double Portocala::Accept (Visitor &v)
{
	return v.VisitPortocala (this);
}
