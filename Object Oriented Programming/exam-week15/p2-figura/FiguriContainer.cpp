#include "FiguriContainer.h"

void FiguriContainer::Add (Figura * f)
{
	figs.push_back (f);
}

void FiguriContainer::ShowAll ()
{
	for (auto x : figs)
	{
		std::cout << x->GetName () << ": " << x->GetCuloare () << "\n";
	}
}

void FiguriContainer::ShowByColor (Color * c)
{
	std::cout << "* Figuri cu culoarea : " << c->GetColor () << "\n";
	for (auto x : figs)
	{
		if (c->HasColor (x->GetCuloare ()))
		{
			std::cout << x->GetName () << '\n';
		}
	}
}
