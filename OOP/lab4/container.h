#pragma once

class container {
public:
	virtual bool Add(int value) = 0;

	virtual bool Del(int value) = 0;

	virtual int Count(int value) = 0;

	virtual bool Exists(int value) = 0;

	virtual int* GetSortedArray() = 0;

	virtual int GetCount() = 0;
};