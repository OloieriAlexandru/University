#pragma once
#include <iostream>

template <typename T>
class List
{
private:
	T * Elemente;
	int Count, Allocated;
public:
	List ()
	{
		Elemente = new T[100];
		Count = 0;
		Allocated = 100;
	}
	~List ()
	{
		delete[] Elemente;
	}
	void Add (T & e)
	{
		if (Count == Allocated)
		{
			Allocated *= 2;
			if (Allocated == 0)
			{
				Allocated = 1;
			}
			T * pointerNou = new T[Allocated];
			for (int i = 0; i < Count; ++i)
			{
				pointerNou[i] = Elemente[i];
			}
			delete[] Elemente;
			Elemente = pointerNou;
		}
		Elemente[Count++] = e;
	}
	T& operator[] (int index)
	{
		if (index < 0 || index >= Count)
		{
			return 0;
		}
		return Elemente[index-1];
	}
	void Sort ()
	{
		bool ok = true;
		while (ok)
		{
			ok = false;
			for (int i = 0; i < Count - 1; ++i)
			{
				if (Elemente[i] > Elemente[i + 1])
				{
					Elemente[i] ^= Elemente[i + 1];
					Elemente[i + 1] ^= Elemente[i];
					Elemente[i] ^= Elemente[i + 1];
					ok = true;
				}
			}
		}
	}
	void Print (std::ostream &out)
	{
		for (int i = 0; i < Count; ++i)
		{
			out << Elemente[i] << ' ';
		}
		out << '\n';
	}
};