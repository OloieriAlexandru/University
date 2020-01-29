#include "EditorVideo.h"

EditorVideo::EditorVideo (std::initializer_list<Video> lst)
{
	for (auto v : lst)
	{
		continut.push_back (v);
	}
}

void EditorVideo::operator=(Video x)
{
	continut.clear ();
	continut.push_back (x);
}

void EditorVideo::afiseaza_intervale_video ()
{
	int s = 0;
	for (auto x : continut)
	{
		std::cout << x.getName() << ' ' << s << " - " << s + x.getLen () << '\n';
		s += x.getLen ();
	}
	std::cout << "Lungime totala video: " << s << '\n';
}

std::vector<Video>::iterator EditorVideo::begin ()
{
	return continut.begin ();
}

std::vector<Video>::iterator EditorVideo::end ()
{
	return continut.end ();
}

EditorVideo & operator+=(EditorVideo & e, Video x)
{
	e.continut.push_back (x);
	return e;
}