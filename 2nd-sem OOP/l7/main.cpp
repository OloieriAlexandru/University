#include <iostream>
#include "map.h"
#include "myClass.h"

myMap<myClass>map1, map2;
myClass obj;

int main ()
{	
	obj.set (10, 20);
	map1["key1"] = obj;
	obj.set (1, 1);
	map1["key2"] = obj;
	obj.set (4, 4);
	map1["key3"] = obj;
	obj.set (30, 1);
	map2["key1"] = obj;
	obj.set (4, 5);
	map2["key3"] = obj;
	std::cout<<"Map1:\n";
	for (auto x : map1)
		std::cout << x.first << ' ' << x.second << '\n';
	std::cout << '\n';
	std::cout << "Map2:\n";
	for (auto x : map2)
		std::cout << x.first << ' ' << x.second << '\n';
	std::cout << '\n';
	std::cout << "Map1 | Map2\n";
	auto map3 = map1 | map2;
	for (auto x : map3)
		std::cout << x.first << ' ' << x.second << '\n';
	std::cout << '\n';
	std::cout << "Map1 & Map2\n";
	auto map4 = map1 & map2;
	for (auto x : map4)
		std::cout << x.first << ' ' << x.second << '\n';
	map3 = map1 | map2;
	std::cout << '\n';
	std::cout << "Assignment operator:\n";
	for (auto x : map3)
		std::cout << x.first << ' ' << x.second << '\n';
	std::cout << '\n';
	std::cout << "Exista cheia \"key1\"? in Map1 " << map1 ("key1") << '\n';
	std::cout << "Exista cheia \"key20\"? in Map2 " << map1 ("key10") << '\n';
	std::cout << "Numarul de elemente din Map1: " << (int)map1 << '\n';

	std::cout << '\n';
	std::cout << "Initializer list:\n";
	myMap<int>init = { {"k1",1},{"k2",2},{"k3",3} };
	for (auto x : init)
		std::cout << x.first << ' ' << x.second << '\n';
	return 0;
}

/*
Intrebari:
	T x;
	memset(&x, 0, sizof(x));
	afecteaza cu ceva obiectul?
*/