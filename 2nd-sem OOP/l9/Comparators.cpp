#include "Comparators.h"

int CompareGreater::CompareElements (void * e1, void * e2)
{
	int *inte1 = (int*)e1;
	int *inte2 = (int*)e2;
	int v1 = *inte1;
	int v2 = *inte2;
	return v1 >= v2;
}
