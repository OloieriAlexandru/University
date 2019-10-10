#pragma once
#include <string>

class Message
{
public:
	virtual void Afiseaza () = 0;
	virtual void Attach (Message *attach) = 0;
	virtual int GetX () = 0;
	virtual int GetY () = 0;
	virtual std::string GetContent () = 0;
};