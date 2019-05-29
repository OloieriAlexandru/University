#include <iostream>
#include "list.h"
#include "bst.h"
#include "container.h"

int main ()
{
	container *c1 = new list ();
	container *c2 = new bst ();

	int n;
	std::cout << "Introdu numarul de elemente (acestea vor fi adaugate atat in bst cat si in lista)\n";
	std::cin >> n;
	for (int i = 0; i < n; ++i)
	{
		int x;
		std::cin >> x;
		c1->Add (x);
		c2->Add (x);
	}

	std::cout << "Introdu numarul de elemente pe care doresti sa le stergi \n";
	std::cin >> n;
	for (int i = 0; i < n; ++i)
	{
		int x;
		std::cout << "Valoarea din lista: ";
		std::cin >> x;
		if (c1->Del (x))
		{
			std::cout << "Valoarea se afla in lista\n";
		}
		else
		{
			std::cout << "Valoarea nu se afla in lista\n";
		}
		std::cout << "Valoarea din bst: ";
		std::cin >> x;
		if (c2->Del (x))
		{
			std::cout << "Valoarea se afla in bst\n";
		}
		else
		{
			std::cout << "Valoarea nu se afla in bst\n";
		}
	}

	std::cout << "Elementele din lista (sortate):";
	int *arr = c1->GetSortedArray ();
	for (int i = 0; i < c1->GetCount(); ++i)
	{
		std::cout << arr[i] << ' ';
	}
	std::cout << '\n';
	delete[] arr;
	
	std::cout << "Elementele din bst (sortate):";
	arr = c2->GetSortedArray ();
	for (int i = 0; i < c2->GetCount(); ++i)
	{
		std::cout << arr[i] << ' ';
	}
	std::cout << '\n';
	delete[] arr;

	system ("pause");
	return 0;
}