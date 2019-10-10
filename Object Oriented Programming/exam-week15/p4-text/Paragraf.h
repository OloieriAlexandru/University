#pragma once
#include "Text.h"
#include <vector>

class Paragraf : public Text
{
private:
	std::vector<Text*>txts;
public:
	void Add (Text *t);

	// Inherited via Text
	virtual void Afiseaza () override;
};