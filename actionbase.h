#ifndef ACTIONBASE_H
#define ACTIONBASE_H

class ActionBase
{
	public:

	ActionBase();
	virtual ~ActionBase();

	virtual void OnTick() = 0;
};

#endif