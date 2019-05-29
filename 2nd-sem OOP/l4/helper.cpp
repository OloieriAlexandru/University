#include "helper.h"

void helper::sort (int * arr, int cnt)
{
	bool ok = true;
	while (ok)
	{
		ok = false;
		for (int i = 0; i < cnt - 1; ++i)
		{
			if (arr[i] > arr[i + 1])
			{
				arr[i] ^= arr[i + 1];
				arr[i + 1] ^= arr[i];
				arr[i] ^= arr[i + 1];
				ok = true;
			}
		}
	}
}
