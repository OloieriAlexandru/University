#include "WhiteBoard.h"

WhiteBoard::WhiteBoard (Message * parent, int x, int y, std::string text, std::string culoare)
{
	this->parent = parent;
	this->x = x;
	this->y = y;
	this->text = text;
	this->culoare = culoare;
}

void WhiteBoard::Afiseaza ()
{
	std::cout << '(' << x << ',' << y << ") " << text << ", culoare: " << culoare;
	if (parent)
		std::cout << ", Mesaj parinte: " << parent->GetContent () << '\n';
	else
		std::cout << '\n';
	for (auto x : alteMesaje)
	{
		x->Afiseaza ();
	}
}

void WhiteBoard::Attach (Message * attach)
{
	alteMesaje.push_back (attach);
}

int WhiteBoard::GetX ()
{
	if (parent)
		return parent->GetX () + x;
	return x;
}

int WhiteBoard::GetY ()
{
	if (parent)
		return parent->GetY () + y;
	return y;
}

std::string WhiteBoard::GetContent ()
{
	return text;
}
