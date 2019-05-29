#include "Grepfruit.h"

Grepfruit::Grepfruit (int dim, std::string cul)
{
	dimensiune = dim;
	culoare = cul;
}

double Grepfruit::Accept (Visitor &v)
{
	return v.VisitGrepfruit (this);
}
