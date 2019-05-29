#pragma once
#include <cstring>
#include <iostream>
#include "Comparators.h"

template <class T>
class Array;

template<class T>
class ArrayIterator
{
private:
	int Current;
	Array<T> *it;
public:
	ArrayIterator ()
	{
		Current = 0;
	}
	ArrayIterator& operator++()
	{
		if (it->Size > Current)
			++Current;
		return *this;
	}
	ArrayIterator& operator--()
	{
		if (Current > 0)
			--Current;
		return *this;
	}
	bool operator=(ArrayIterator<T> &itt)
	{
		Current = itt;
		it = itt.it;
		return true;
	}
	bool operator!=(ArrayIterator<T> &itt)
	{
		if (itt.it == it)
			return Current != itt.Current;
		throw new std::exception ("Iteratorii nu pot fi comparati!\n");
	}
	T* operator *()
	{
		return it->List[Current];
	}
	void SetPointer (Array<T> *itt)
	{
		it = itt;
	}
	void SetCurrent (int Curr)
	{
		Current = Curr;
	}
};

template<class T>
class Array
{
private:
	T** List;
	int Capacity;
	int Size;

	void Resize (int numberOfNewElements)
	{
		int minimumRequiredSize = Size + numberOfNewElements;
		while (Capacity < minimumRequiredSize)
		{
			T** newE = new T*[Capacity ? Capacity << 1 : 1];
			memset (newE, 0, sizeof (newE));
			for (int i = 0; i < Size; ++i)
				newE[i] = List[i];
			delete[] List;
			List = newE;
			Capacity = (Capacity ? Capacity << 1 : 1);
		}
	}
	void Swap (T* &a ,T* &b)
	{
		T* c = a;
		a = b;
		b = c;
	}
	friend class ArrayIterator<T>;
public:
	Array ()
	{
		List = 0;
		Capacity = Size = 0;
	}
	~Array ()
	{
		delete[] List;
	}
	Array (int capacity)
	{
		List = new T*[capacity];
		memset (List, 0, sizeof (List));
		Capacity = capacity;
		Size = 0;
	}
	Array (const Array<T> &otherArray)
	{
		List = new T*[otherArray.Capacity];
		memset (List, 0, sizeof (List));
		Capacity = otherArray.Capacity;
		Size = otherArray.Size;
		for (int i = 0; i < Size; ++i)
		{
			List[i] = new T;
			*List[i] = *otherArray.List[i];
		}
	}
	T& operator[] (int index)
	{
		if (index < 0 || index >= Size)
			throw std::out_of_range ("Index invalid");
		return *List[index];
	}
	const Array<T>& operator+=(const T &newElem)
	{
		Resize (1);
		List[Size] = new T;
		*List[Size++] = newElem;
		return *this;
	}
	const Array<T>& Insert (int index, const T &newElem)
	{
		if (index < 0 || index > Size)
			throw std::out_of_range ("Index invalid");
		Resize (1);
		List[Size] = new T;
		for (int i = Size; i > index; --i)
			*List[i] = *List[i - 1];
		*List[index] = newElem;
		++Size;
		return *this;
	}
	const Array<T>& Insert (int index, const Array<T>otherArray)
	{
		if (index < 0 || index > Size)
			throw  std::out_of_range ("Index invalid");
		Resize (otherArray.Size);
		for (int i = 0; i < otherArray.Size; ++i)
			List[Size + i] = new T;
		for (int i = Size + otherArray.Size - 1, j = 0; j < otherArray.Size; --i, ++j)
			*List[i] = *List[i - otherArray.Size];
		for (int j = 0; j < otherArray.Size; ++j)
			*List[index + j] = *otherArray.List[j];
		Size = Size + otherArray.Size - 1;
		return *this;
	}
	const Array<T>& Delete (int index)
	{
		if (index < 0 || index >= Size)
			throw new std::out_of_range ("Index invalid");
		for (int i = index; i < Size - 1; ++i)
			List[i] = List[i + 1];
		--Size;
		return *this;
	}
	bool operator=(const Array<T>& otherArray)
	{
		if (List)
			delete[] List;
		List = new T*[otherArray.Capacity];
		memset (List, 0, sizeof (List));
		Capacity = otherArray.Capacity;
		Size = otherArray.Size;
		for (int i = 0; i < Size; ++i)
			List[i] = otherArray.List[i];
		return true;
	}
	void Sort ()
	{
		bool sorted = false;
		while (!sorted)
		{
			sorted = true;
			for (int i = 0; i < Size - 1; ++i)
				if (*List[i] > *List[i + 1])
				{
					Swap (List[i], List[i + 1]);
					sorted = false;
				}
		}
	}
	void Sort (int (*compare)(const T&, const T&))
	{
		bool sorted = false;
		while (!sorted)
		{
			sorted = true;
			for (int i = 0; i < Size - 1; ++i)
				if (!compare (*List[i], *List[i + 1]))
				{
					Swap (List[i], List[i + 1]);
					sorted = false;
				}
		}
	}
	void Sort (Compare *comparator)
	{
		bool sorted = false;
		while (!sorted)
		{
			sorted = true;
			for (int i=0;i<Size-1;++i)
				if (!comparator->CompareElements ((void*)List[i], (void*)List[i + 1]))
				{
					Swap (List[i], List[i + 1]);
					sorted = false;
				}
		}
	}
	int BinarySearch (const T& elem)
	{
		Sort ();
		int mid, st = 0, dr = Size - 1;
		while (st <= dr)
		{
			mid = (st + dr) / 2;
			if (*List[mid] == elem)
				return mid;
			if (*List[mid] > elem)
				dr = mid - 1;
			else
				st = mid + 1;
		}
		return -1;
	}
	int BinarySearch (const T& elem, int (*compare)(const T&, const T&))
	{
		Sort (compare);
		int mid, st = 0, dr = Size - 1;
		while (st <= dr)
		{
			mid = (st + dr) / 2;
			if (*List[mid] == elem)
				return mid;
			if (*List[mid] > elem)
				dr = mid - 1;
			else
				st = mid + 1;
		}
		return -1;
	}
	int BinarySearch (const T& elem, Compare *comparator)
	{
		Sort (comparator);
		int mid, st = 0, dr = Size - 1;
		while (st <= dr)
		{
			mid = (st + dr) / 2;
			if (comparator->CompareElements((void*)&mid,(void*)List[mid]))
				return mid;
			if (*List[mid] > elem)
				dr = mid - 1;
			else
				st = mid + 1;
		}
		return -1;
	}
	int Find (const T& elem)
	{
		for (int i = 0; i < Size; ++i)
			if (*List[i] == elem)
				return i;
		return -1;
	}
	int Find (const T& elem, int (*compare)(const T&, const T&))
	{
		for (int i = 0; i < Size; ++i)
			if (compare (*List[i], elem))
				return i;
		return -1;
	}
	int Find (const T& elem, Compare *comparator)
	{
		for (int i = 0; i < Size; ++i)
			if (comparator->CompareElements ((void*)*List[i], (void*)elem))
				return i;
		return -1;
	}
	int GetSize ()
	{
		return Size;
	}
	int GetCapacity ()
	{
		return Capacity;
	}
	ArrayIterator<T> GetBeginIterator ()
	{
		ArrayIterator<T> it;
		it.SetPointer (this);
		return it;
	}
	ArrayIterator<T> GetEndIterator ()
	{
		ArrayIterator<T> it;
		it.SetPointer (this);
		it.SetCurrent (Size);
		return it;
	}
	ArrayIterator<T> begin ()
	{
		return GetBeginIterator ();
	}
	ArrayIterator<T> end ()
	{
		return GetEndIterator ();
	}
	void Print (std::ostream &out)
	{
		for (int i = 0; i < Size; ++i)
			out << *List[i] << ' ';
		out << '\n';
	}
};