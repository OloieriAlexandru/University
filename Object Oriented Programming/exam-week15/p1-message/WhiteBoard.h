#pragma once
#include "Message.h"
#include <string>
#include <iostream>
#include <vector>

class WhiteBoard : public Message
{
private:
	int x;
	int y;
	std::string text;
	std::string culoare;
	std::vector<Message*>alteMesaje;
	Message *parent;
public:
	WhiteBoard (Message *parent, int x, int y, std::string text, std::string culoare);

	// Inherited via Message
	virtual void Afiseaza () override;
	virtual void Attach (Message * attach) override;
	virtual int GetX () override;
	virtual int GetY () override;
	virtual std::string GetContent () override;

};