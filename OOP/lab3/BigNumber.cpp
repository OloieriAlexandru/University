#include "BigNumber.h"

BigNumber::BigNumber()
{
	CharactersCount = 0;
	memset(Number, 0, sizeof(Number));
}

BigNumber::BigNumber(int value)
{
	CharactersCount = 0;
	memset(Number, 0, sizeof(Number));
	while (value)
	{
		Number[CharactersCount++] = value % 10;
		value /= 10;
	}
}

BigNumber::BigNumber(const char * number)
{
	CharactersCount = strlen(number);
	memset(Number, 0, sizeof(Number));
	for (int i = 0; i < CharactersCount; ++i)
	{
		Number[i] = number[CharactersCount - i - 1] - '0';
	}
}

BigNumber::BigNumber(const BigNumber & number)
{
	CharactersCount = number.CharactersCount;
	for (int i = 0; i < CharactersCount; ++i)
	{
		Number[i] = number.Number[i];
	}
}

bool BigNumber::Set(int value)
{
	if (value < 0)
	{
		return 0;
	}
	memset(Number, 0, sizeof(Number));
	CharactersCount = 0;
	while (value)
	{
		Number[CharactersCount++] = value % 10;
		value /= 10;
	}
	return true;
}

bool BigNumber::Set(const char * number)
{
	CharactersCount = strlen(number);
	for (int i = 0; i < CharactersCount; ++i)
	{
		if (!(number[i] >= '0' && number[i] <= '9'))
		{
			return false;
		}
	}
	if (CharactersCount > 256)
	{
		return false;
	}
	memset(Number, 0, sizeof(Number));
	for (int i = 0; i < CharactersCount; ++i)
	{
		Number[i] = number[CharactersCount - i - 1] - '0';
	}
	return true;
}

BigNumber BigNumber::operator+(const BigNumber & number)
{
	BigNumber ans;
	ans.CharactersCount = CharactersCount > number.CharactersCount ? CharactersCount : number.CharactersCount;
	int t = 0, v;
	for (int i = 0; i < ans.CharactersCount; ++i)
	{
		v = t + Number[i] + number.Number[i];
		ans.Number[i] = v % 10;
		t = v / 10;
	}
	if (t)
	{
		ans.Number[ans.CharactersCount++] = 1;
	}
	return ans;
}

BigNumber BigNumber::operator*(const BigNumber & number)
{
	BigNumber ans;
	for (int i = 0; i < CharactersCount; ++i)
	{
		for (int j = 0; j < number.CharactersCount; ++j)
		{
			ans.Number[i + j] += Number[i] * number.Number[j];
		}
	}
	ans.CharactersCount = CharactersCount + number.CharactersCount - 1;
	int t = 0, v;
	for (int i = 0; i < ans.CharactersCount; ++i)
	{
		v = ans.Number[i] + t;
		ans.Number[i] = v % 10;
		t = v / 10;
	}
	while (t)
	{
		ans.Number[ans.CharactersCount++] = v % 10;
		v /= 10;
	}
	return ans;
}

BigNumber BigNumber::operator-(const BigNumber & number)
{
	BigNumber ans;
	if (*this < number)
	{
		return ans;
	}
	int t = 0, v;
	ans.CharactersCount = CharactersCount;
	for (int i = 0; i < CharactersCount; ++i)
	{
		v = Number[i] - t - (i < number.CharactersCount ? number.Number[i] : 0);
		if (v < 0)
		{
			t = 1;
			ans.Number[i] = v + 10;
		}
		else
		{
			t = 0;
			ans.Number[i] = v;
		}
	}
	while (ans.Number[ans.CharactersCount-1] == 0)
	{
		--ans.CharactersCount;
	}
	return ans;
}

BigNumber BigNumber::operator/(const BigNumber & number)
{
	BigNumber one(1);
	BigNumber ans;
	BigNumber copy(*this);
	while (copy > number)
	{
		ans = ans + one;
		copy = copy - number;
	}
	return ans;
}

BigNumber::operator int()
{
	int ret = 0;
	if (CharactersCount <= 9)
	{
		for (int i = CharactersCount - 1; i >= 0; --i)
		{
			ret = ret * 10 + Number[i];
		}
	}
	return ret;
}

char BigNumber::operator[](int index)
{
	if (index < 0 || index >= CharactersCount)
	{
		return 0;
	}
	return Number[index];
}

BigNumber BigNumber::operator()(int start, int end)
{
	BigNumber ans;
	if (start < 0 || start >= CharactersCount)
	{
		return ans;
	}
	if (start > end)
	{
		return ans;
	}
	if (end < 0 || end >= CharactersCount)
	{
		return ans;
	}
	ans.CharactersCount = end - start + 1;
	end = CharactersCount - end - 1;
	start = CharactersCount - start - 1;
	for (int i = ans.CharactersCount -1, j = start; j >= end; --j, --i)
	{
		ans.Number[i] = Number[j];
	}
	return ans;
}

bool operator==(const BigNumber & n1, const BigNumber & n2)
{
	if (n1.CharactersCount != n2.CharactersCount)
	{
		return false;
	}
	for (int i = 0; i < n1.CharactersCount; ++i)
	{
		if (n1.Number[i] != n2.Number[i])
		{
			return false;
		}
	}
	return true;
}

bool operator!=(const BigNumber & n1, const BigNumber & n2)
{
	return !(n1 == n2);
}

bool operator<(const BigNumber & n1, const BigNumber & n2)
{
	if (n1.CharactersCount < n2.CharactersCount)
	{
		return true;
	}
	if (n1.CharactersCount > n2.CharactersCount)
	{
		return false;
	}
	int i = n1.CharactersCount - 1;
	while (i >= 0 && n1.Number[i] == n2.Number[i])
	{
		--i;
	}
	if (i == -1)
	{
		return false;
	}
	return n1.Number[i] < n2.Number[i];
}

bool operator>(const BigNumber & n1, const BigNumber & n2)
{
	if (n1.CharactersCount > n2.CharactersCount)
	{
		return true;
	}
	if (n1.CharactersCount < n2.CharactersCount)
	{
		return false;
	}
	int i = n1.CharactersCount - 1;
	while (i >= 0 && n1.Number[i] == n2.Number[i])
	{
		--i;
	}
	if (i == -1)
	{
		return false;
	}
	return n1.Number[i] > n2.Number[i];
}

bool operator>=(const BigNumber & n1, const BigNumber & n2)
{
	return !(n1 < n2);
}

bool operator<=(const BigNumber & n1, const BigNumber & n2)
{
	return !(n1 > n2);
}

std::ostream & operator<<(std::ostream & out, BigNumber & n)
{
	for (int i = n.CharactersCount - 1; i >= 0; --i)
	{
		out << (int)n.Number[i];
	}
	out << '\n';
	return out;
}
