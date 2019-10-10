#pragma once
#include <vector>
#include <functional>

template <class T>
int Partition (std::vector<T> &lista, int l, int r, std::function<int (T &e1, T &e2)> Compare)
{
	T pivot = lista[r];
	int i = l - 1;
	for (int j = l; j <= r - 1; ++j)
	{
		if (Compare (lista[j], pivot))
		{
			++i;
			auto aux = lista[i];
			lista[i] = lista[j];
			lista[j] = aux;
		}
	}
	auto aux = lista[i + 1];
	lista[i + 1] = lista[r];
	lista[r] = aux;
	return i + 1;
}

template <class T>
void SortRec (std::vector<T> &lista, int l, int r, std::function<int (T &e1, T &e2)> Compare)
{
	if (l < r)
	{
		int pivot = Partition (lista, l, r, Compare);
		SortRec (lista, l, pivot-1, Compare);
		SortRec (lista, pivot + 1, r, Compare);
	}
}

template <class T>
void Sort1 (std::vector<T> &lista, int (*Compare)(T &e1, T &e2))
{
	std::cout << "Apel cu pointer la functie\n";
	std::function<int (T &e1, T &e2)> Cmp = Compare;
	SortRec (lista, 0, lista.size () - 1, Cmp);
}

template <class T>
void Sort (std::vector<T> &lista, std::function<int(T &e1, T &e2)> Compare)
{
	std::cout << "Apel cu std::function\n";
	SortRec (lista, 0, lista.size () - 1, Compare);
}
