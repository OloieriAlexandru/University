#pragma once
#include <string>
#include <utility>
#include <initializer_list>
#include "list.h"

const static int tableSize = 100003;
const static int modValue = 1000000007;
const static int hashBase = 31;

template <class TIterM>
class mapIterator
{
public:
	list<TIterM>* hashTable;
	listIterator<TIterM> curr;
	int currHash;
	mapIterator& operator++()
	{
		curr.operator++();
		if (!(curr.operator!=(hashTable[currHash].end ())))
		{
			++currHash;
			while (currHash < tableSize && !(hashTable[currHash].begin().operator!=(hashTable[currHash].end ()))) ++currHash;
			if (currHash < tableSize)
				curr = hashTable[currHash].begin ();
			else
				curr.curr = 0;
		}
		return *this;
	}
	bool operator != (mapIterator &it)
	{
		return currHash != it.currHash;
	}
	std::pair<std::string, TIterM> operator* ()
	{
		return *curr;
	}
};

template <class TMap>
class myMap
{
private:
	int count;
	list<TMap>*hashTable;

	friend class mapIterator<TMap>;

	int getHash (std::string key)
	{
		int ans = 0;
		int pow = 1;
		for (auto x : key)
		{
			ans = (ans + 1LL * x*pow) % tableSize;
			pow = (1LL * 31) % modValue;
		}
		return ans;
	}
	void allocateMemory ()
	{
		hashTable = new list<TMap>[tableSize];
		memset (hashTable, 0, sizeof (hashTable));
	}
	void copyAllElements (const myMap<TMap> &obj)
	{
		for (int i = 0; i < tableSize; ++i)
		{
			listIterator<TMap> it = obj.hashTable[i].begin ();
			while (it.operator!=(obj.hashTable[i].end ()))
			{
				insert (it.operator*().first, it.operator*().second);
				++it;
			}
		}
	}
public:
	myMap ()
	{
		count = 0;
		allocateMemory ();
	}
	myMap (std::initializer_list<std::pair<std::string, TMap>> initValues)
	{
		count = 0;
		allocateMemory ();
		for (auto x : initValues)
		{
			++count;
			insert (x);
		}
	}
	myMap (const myMap<TMap> &obj)
	{
		allocateMemory ();
		count = obj.count;
		copyAllElements (obj);
	}
	~myMap ()
	{
		delete[] hashTable;
	}
	void insert (std::string key, TMap value)
	{
		++count;
		int hash = getHash (key);
		hashTable[hash][key] = value;
	}
	void insert (std::pair<std::string, TMap> e)
	{
		insert (e.first, e.second);
	}
	bool remove (std::string key)
	{
		int hash = getHash (key);
		if (hashTable[hash].remove (key))
		{
			--count;
			return true;
		}
		return false;
	}
	bool operator ()(std::string key)
	{
		int hash = getHash (key);
		return hashTable[hash].exists (key);
	}
	operator int ()
	{
		return count;
	}
	TMap& operator[] (std::string key)
	{
		int hash = getHash (key);
		if (!hashTable[hash].exists (key))
			++count;
		return hashTable[hash][key];
	}
	TMap& operator[] (const char *key)
	{
		std::string sKey (key);
		return (*this)[sKey];
	}
	mapIterator<TMap> begin ()
	{
		mapIterator<TMap> it;
		it.hashTable = &hashTable[0];
		it.currHash = 0;
		while (it.currHash < tableSize && !(it.hashTable[it.currHash].begin ().operator!=(it.hashTable[it.currHash].end ())))
			++it.currHash;
		if (it.currHash < tableSize)
			it.curr = it.hashTable[it.currHash].begin ();
		else
			it.curr.curr = 0;
		return it;
	}
	mapIterator<TMap> end ()
	{
		mapIterator<TMap> it;
		it.hashTable = &hashTable[0];
		it.currHash = tableSize;
		it.curr.curr = 0;
		return it;
	}
	myMap<TMap> operator=(const myMap<TMap> &obj)
	{
		count = obj.count;
		allocateMemory ();
		copyAllElements (obj);
		return *this;
	}
	friend myMap<TMap> operator &(const myMap<TMap> &a, const myMap<TMap> &b)
	{
		myMap<TMap>ans;
		for (int i = 0; i < tableSize; ++i)
		{
			if (a.hashTable[i].empty () || b.hashTable[i].empty ()) continue;
			a.hashTable[i].sort ();
			b.hashTable[i].sort ();
			listIterator<TMap> ita = a.hashTable[i].begin ();
			listIterator<TMap> itb = b.hashTable[i].begin ();
			while (ita.operator!=(a.hashTable[i].end ()) && itb.operator!=(b.hashTable[i].end ()))
			{
				if (ita.operator*().first == itb.operator*().first)
				{
					if (ita.operator*().second > itb.operator*().second)
					{
						ans.insert (std::make_pair (ita.operator*().first, ita.operator*().second));
						++ita, ++itb;
						continue;
					}
					ans.insert (std::make_pair (itb.operator*().first, itb.operator*().second));
					++ita, ++itb;
					continue;
				}
				if (ita.operator*().first < itb.operator*().first)
				{
					++ita;
					continue;
				}
				++itb;
			}
		}
		return ans;
	}
	friend myMap<TMap> operator |(const myMap<TMap> &a, const myMap<TMap> &b)
	{
		myMap<TMap>ans;
		for (int i = 0; i < tableSize; ++i)
		{
			if (a.hashTable[i].empty() && b.hashTable[i].empty ()) continue;
			a.hashTable[i].sort ();
			b.hashTable[i].sort ();
			listIterator<TMap> ita = a.hashTable[i].begin ();
			listIterator<TMap> itb = b.hashTable[i].begin ();
			while (ita.operator!=(a.hashTable[i].end ()) && itb.operator!=(b.hashTable[i].end ()))
			{
				if (ita.operator*().first == itb.operator*().first)
				{
					if (ita.operator*().second > itb.operator*().second)
					{
						ans.insert (std::make_pair (ita.operator*().first, ita.operator*().second));
						++ita, ++itb;
						continue;
					}
					ans.insert (std::make_pair (itb.operator*().first, itb.operator*().second));
					++ita, ++itb;
					continue;
				}
				if (ita.operator*().first < itb.operator*().first)
				{
					ans.insert (std::make_pair (ita.operator*().first, ita.operator*().second));
					++ita;
					continue;
				}
				ans.insert (std::make_pair (itb.operator*().first, itb.operator*().second));
				++itb;
			}
			while (ita.operator!=(a.hashTable[i].end ()))
			{
				ans.insert (std::make_pair (ita.operator*().first, ita.operator*().second));
				++ita;
			}
			while (itb.operator!=(b.hashTable[i].end ()))
			{
				ans.insert (std::make_pair (itb.operator*().first, itb.operator*().second));
				++itb;
			}
		}
		return ans;
	}
};