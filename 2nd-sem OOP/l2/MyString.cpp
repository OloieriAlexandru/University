#include "MyString.h"

void MyString::Resize (unsigned expectedLength)
{
	while (AllocatedSize <= expectedLength)
		AllocatedSize *= 2;
	char *newString = new char[AllocatedSize];
	for (unsigned i = 0; i < AllocatedSize; ++i)
		newString[i] = 0;
	for (unsigned i = 0; i < Size; ++i)
		newString[i] = sir[i];
	delete[] sir;
	sir = newString;
}

MyString::MyString ()
{
	sir = new char[16];
	memset (sir, 0, 16);
	AllocatedSize = 16;
	Size = 0;
}

MyString::MyString (const char * text)
{
	int ln = strlen (text);
	AllocatedSize = (ln < 16 ? 16 : ln);
	sir = new char[AllocatedSize];
	Size = ln;
	memset (sir, 0, sizeof (sir));
	for (int i = 0; i < ln; ++i)
		sir[i] = text[i];
}

MyString::~MyString ()
{
	delete[] sir;
}

unsigned int MyString::GetSize ()
{
	return Size;
}

void MyString::Set (const char * text)
{
	int ln = strlen (text);
	Resize (ln);
	for (int i = 0; i < ln; ++i)
		sir[i] = text[i];
	Size = ln;
}

void MyString::Set (MyString & m)
{
	Set (m.sir);
}

void MyString::Add (const char * text)
{
	int ln = strlen (text);
	Resize (Size + ln);
	for (int i = Size, j = 0; j < ln; ++j, ++i)
		sir[i] = text[j];
	Size = Size + ln;
}

void MyString::Add (MyString & m)
{
	Add (m.sir);
}

const char * MyString::GetText ()
{
	return sir;
}

MyString * MyString::SubString (unsigned int start, unsigned int size)
{
	if (start >= Size)
		return NULL;
	if (start + size >= Size)
		return NULL;
	MyString *ans = new MyString ();
	ans->Resize (size);
	for (unsigned i = 0; i < size; ++i)
		ans->sir[i] = sir[start + i];
	Size = size;
	return ans;
}

bool MyString::Delete (unsigned int start, unsigned int size)
{
	if (start >= Size)
		return false;
	if (start + size >= Size)
		return false;
	int endd = start + size;
	for (int i = start; i < endd; ++i)
		sir[i] = sir[i + 1];
	sir[endd] = 0;
	return true;
}

int MyString::Compare (const char * text)
{
	unsigned lgt = strlen (text);
	unsigned lgM = (lgt < Size ? lgt : Size);
	for (unsigned i = 0; i < lgM; ++i)
	{
		if (sir[i] < text[i])
			return -1;
		if (sir[i] > text[i])
			return 1;
	}
	if (lgt == lgM)
		return 0;
	if (lgM < lgt)
		return -1;
	return 1;
}

int MyString::Compare (MyString & m)
{
	return Compare (m.sir);
}

char MyString::GetChar (unsigned int index)
{
	if (index >= Size)
		return 0;
	return sir[index];
}

bool MyString::Insert (unsigned int index, const char * text)
{
	if (index >= Size)
		return false;
	unsigned ln = strlen (text);
	Resize (Size + ln);
	int finalLength = Size + ln;
	for (unsigned i = finalLength - 1; i > index; --i)
		sir[i] = sir[i - ln];
	for (unsigned i = index, j = 0; j < ln; ++i, ++j)
		sir[i] = text[j];
	Size = Size + ln;
	return true;
}

bool MyString::Insert (unsigned int index, MyString & m)
{
	return Insert (index, m.sir);
}

int MyString::Find (const char * text)
{
	unsigned ln = strlen (text);
	if (ln > Size)
		return -1;
	for (unsigned i = 0; i < Size - ln; ++i)
	{
		bool ok = true;
		for (unsigned j = 0; j < ln; ++j)
			if (sir[i + j] != text[j])
			{
				ok = false;
				break;
			}
		if (ok)
			return (int)i;
	}
	return -1;
}

int MyString::FindLast (const char * text)
{
	unsigned ln = strlen (text);
	if (ln > Size)
		return -1;
	for (unsigned i = Size - ln; i >= 0; --i)
	{
		bool ok = true;
		for (unsigned j=0;j<ln;++j)
			if (sir[i + j] != text[j])
			{
				ok = false;
				break;
			}
		if (ok)
			return (int)i;
	}
	return -1;
}

std::ostream & operator<<(std::ostream & out, const MyString & str)
{
	out << str.sir << '\n';
	return out;
}
