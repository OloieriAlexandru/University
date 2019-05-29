#include <iostream>
#include "BigNumber.h"

int main ()
{
	BigNumber a (1), b (3);
	BigNumber cmp1 (10), cmp2 (11), cmp3 (12), cmp4 (10);
	std::cout << (a == b) << ' ' << (a != b) << '\n';
	std::cout << a + b;
	std::cout << (cmp1 >= cmp2) << ' ' << (cmp1 >= cmp4) << " >= \n";
	std::cout << (cmp1 <= cmp4) << ' ' << (cmp3 <= cmp2) << " <= \n";
	system ("pause");
	return 0;
}