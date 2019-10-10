#include "Camera.h"

Camera::Camera (std::string m, std::string n, bool l)
{
	miros = m;
	nume = n;
	lumina = l;
}

void Camera::AddSpatiu (SpatiuInchis * si)
{
	spaces.push_back (si);
}

void Camera::PrintInfo ()
{
	std::cout << nume << " are " << spaces.size () << " camere, miros: " << miros << ", " << (lumina == true ? "luminos" : "intunecat")<<'\n';
	for (auto x : spaces)
		x->PrintInfo ();
}
