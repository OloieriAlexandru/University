#include <iostream>
#include "array.h"

Array<int> arr;
int a[35];

int main ()
{
	std::cout << "Capacitatea containerului dupa fiecare inserare:\n";
	for (int i = 0; i < 35; ++i)
	{
		a[i] = i + 1;
		a[i] %= 25;
		arr += a[i];
		std::cout << arr.GetCapacity () << ", ";
	}
	std::cout << '\n';
	std::cout << "Elementele din container:\n";
	arr.Print (std::cout);
	arr.Sort ();	
	std::cout << "Elementele din container dupa apelarea functiei sort fara parametri:\n";
	arr.Print (std::cout);
	std::cout << "Elementele dupa apelul \"arr.Delete (arr.Find(1)):\n";
	arr.Delete (arr.Find (1));
	arr.Print (std::cout);
	std::cout << "Elementele iterate cu ajutorul for (auto x:...):\n";
	for (auto x : arr)
		std::cout << *x << ' ';
	std::cout << '\n';
	arr.Sort ([](const int &a, const int &b)->int {
		return a >= b; });
	std::cout << "Elementele dupa apelul functiei sort cu lambda expression:\n";
	for (auto x : arr)
		std::cout << *x << ' ';
	std::cout << '\n';
	if (arr.BinarySearch (18) != -1)
		std::cout << "Elementul 18 gasit cu BinarySearch\n";
	else
		std::cout << "Elementul 18 nu a fost gasit cu BinarySearch\n";
	if (arr.BinarySearch (54) != -1)
		std::cout << "Elementul 54 gasit cu BinarySearch\n";
	else
		std::cout << "Elementul 54 nu a fost gasit cu BinarySearch\n";
	std::cout << "Elementele din container:\n";
	arr.Print (std::cout);
	std::cout << "arr.Insert(1,105):\n";
	arr.Insert (1, 105);
	arr.Print (std::cout);
	std::cout << "Elementele containerului arr2:\n";
	Array<int>arr2;
	for (int i = 100; i <= 105; ++i)
		arr2 += i;
	arr2.Print (std::cout);
	std::cout << "Elementele din container:\n";
	arr.Print (std::cout);
	std::cout << "arr.insert(10,arr2):\n";
	arr.Insert (10, arr2);
	arr.Print (std::cout);
	try {
		arr[1000] = 5555;
	}
	catch (std::exception &exp)
	{
		std::cout << exp.what () << '\n';
	}
	std::cout << "Elementele din container dupa apelarea functiei sort cu un obiect de tip Compare:\n";
	CompareGreater cmp;
	arr.Sort (&cmp);
	arr.Print (std::cout);
	return 0;
}