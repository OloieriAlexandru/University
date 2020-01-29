#pragma once
#include "Video.h"
#include <vector>
#include <initializer_list>

class EditorVideo
{
private:
	std::vector<Video>continut;
public:
	EditorVideo (std::initializer_list<Video> lst);

	friend EditorVideo& operator +=(EditorVideo &e, Video x);
	void operator =(Video x);
	void afiseaza_intervale_video ();

	std::vector<Video>::iterator begin ();
	std::vector<Video>::iterator end ();
};