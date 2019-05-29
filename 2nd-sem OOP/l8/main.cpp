#include <iostream>
#include <vector>
#include "sort.h"
#include "myClass.h"

int n, x;

int main ()
{
	std::vector<int>v ({ 10, 20, 1, 5, -1 , 241, 42, 100 });
	std::cout << "Vectorul initial: \n";
	for (auto x : v)
		std::cout << x << ' ';
	std::cout << '\n';
	Sort<int>(v, (std::function<int(int &,int&)>)[](int &a, int &b)->int {
		return (int)(a > b);
	});
	std::cout << "Vectorul dupa sortarea cu lambda (int)(a > b):\n";
	for (auto x : v)
		std::cout << x << ' ';
	std::cout << '\n';

	std::vector<myClass> v2 ({ myClass (2,3,10),myClass (2,3,4),myClass (1,1,6),myClass(4, 1, 1) });
	std::cout << "Vectorul cu obiecte custom: \n";
	for (auto x : v2)
		std::cout << x << '\n';
	Sort1<myClass> (v2, [](myClass &m1, myClass &m2)->int {
		int v1 = m1.getX () + m1.getY ();
		int v2 = m2.getX () + m2.getY ();
		if (v1 == v2)
			return m1.getW () < m2.getW ();
		return v1 < v2;
	});
	std::cout << "Vectorul cu obiecte custom dupa aplicarea sortarii:\n";
	for (auto x : v2)
		std::cout << x << '\n';
	return 0;
}