#pragma once

class Compare
{
public:
	virtual int CompareElements (void *e1, void *e2) = 0;
};

class CompareGreater : public Compare
{
	virtual int CompareElements (void * e1, void * e2) override;
};