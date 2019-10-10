#include "Sticker.h"

Sticker::Sticker (Message * parent, int x, int y, std::string text, std::string culoare)
{
	this->x = x;
	this->y = y;
	this->text = text;
	this->culoare = culoare;
	this->parent = parent;
}

void Sticker::Afiseaza ()
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

void Sticker::Attach (Message * attach)
{
	alteMesaje.push_back (attach);
}

int Sticker::GetX ()
{
	if (parent)
		return parent->GetX () + x;
	return x;
}

int Sticker::GetY ()
{
	if (parent)
		return parent->GetY () + y;
	return y;
}

std::string Sticker::GetContent ()
{
	return text;
}
