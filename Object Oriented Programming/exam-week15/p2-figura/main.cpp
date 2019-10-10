#include "Cerc.h"
#include "Dreptunghi.h"
#include "Patrat.h"
#include "FiguriContainer.h"
#include "Color.h"

int main ()
{
	FiguriContainer fig;
	fig.Add (new Cerc ("rosu"));
	fig.Add (new Patrat ("verde"));
	fig.Add (new Drepunghi ("rosu"));
	fig.ShowAll ();
	fig.ShowByColor (new Color ("rosu"));
}