#include "Lamaie.h"

Lamaie::Lamaie (int dim, std::string cul)
{
	dimensiune = dim;
	culoare = cul;
}

double Lamaie::Accept (Visitor &v)
{
	return v.VisitLamaie (this);
}
