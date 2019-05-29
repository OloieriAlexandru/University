#pragma once
#include "Contact.h"
#include "Prieten.h"
#include <vector>
#include <iostream>

class Agenda
{
private:
	std::vector<Contact*> cts;
public:
	
	Contact* Find (std::string name);
	std::vector<Prieten> GetFriends ();
	void Delete (std::string name);
	void Add (Contact* ctc);

	void PrintAll ();
};