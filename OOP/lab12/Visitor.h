#pragma once

class Grepfruit;
class Lamaie;
class Portocala;

class Visitor
{
protected:
	Visitor ();
public:
	virtual double VisitLamaie (Lamaie *l) = 0;
	virtual double VisitPortocala (Portocala *p) = 0;
	virtual double VisitGrepfruit (Grepfruit *g) = 0;
};