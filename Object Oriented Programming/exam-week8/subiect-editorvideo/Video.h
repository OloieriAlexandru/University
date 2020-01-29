#pragma once
#include <string>
#include <iostream>

class Video
{
private:
	std::string nume;
	int lungime;
public:
	Video ();
	Video (std::string nme, int lg);

	friend Video operator |(const Video &v1, const Video &v2);
	friend std::ostream& operator <<(std::ostream &out, const Video &v);

	std::string getName ();
	int getLen ();
};