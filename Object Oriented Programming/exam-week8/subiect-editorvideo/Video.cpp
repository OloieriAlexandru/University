#include "Video.h"

Video operator|(const Video & v1, const Video & v2)
{
	Video ans;
	ans.nume = v1.nume + v2.nume;
	ans.lungime = v1.lungime + v2.lungime;
	return ans;
}

std::ostream & operator<<(std::ostream & out, const Video & v)
{
	out << "Film: " << v.nume;
	out << " (" << v.lungime << ")";
	return out;
}

Video::Video ()
{
	lungime = 0;
}

Video::Video (std::string nme, int lg)
{
	nume = nme;
	lungime = lg;
}

std::string Video::getName ()
{
	return nume;
}

int Video::getLen ()
{
	return lungime;
}
