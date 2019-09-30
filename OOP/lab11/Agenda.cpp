#include "Agenda.h"

Contact* Agenda::Find (std::string name)
{
	int rt = -1;
	for (int i = 0; i < (int)cts.size (); ++i)
	{
		if (cts[i]->GetName () == name)
		{
			rt = i;
			break;
		}
	}
	if (rt != -1)
	{
		return cts[rt];
	}
	return nullptr;
}

std::vector<Prieten> Agenda::GetFriends ()
{
	std::vector<Prieten> ans;
	for (int i = 0; i < (int)cts.size (); ++i)
	{
		if (dynamic_cast<Prieten*>(cts.at(i)) != nullptr)
		{
			Prieten& p = dynamic_cast<Prieten&>(*cts.at (i));
			ans.push_back (p);
		}
	}
	return ans;
}

void Agenda::Delete (std::string name)
{
	int rm = -1;
	for (int i = 0; i < (int)cts.size (); ++i)
	{
		if (cts[i]->GetName () == name)
		{
			rm = i;
			break;
		}
	}
	if (rm != -1)
	{
		cts.erase (cts.begin () + rm);
	}
}

void Agenda::Add (Contact* ctc)
{
	cts.push_back (ctc);
}

void Agenda::PrintAll ()
{
	std::cout << "Numarul de contacte: " << cts.size () << '\n';
	for (int i = 0; i < (int)cts.size (); ++i)
	{
		std::cout << (i + 1) << ": " << cts[i]->GetName () << '\n';
	}
}
