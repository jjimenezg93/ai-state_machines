#ifndef _ACTION_RAND_COLOR_H
#define _ACTION_RAND_COLOR_H

#include "action.h"

class ActionRandColor: public Action {
public:
	virtual void Start();
	virtual void Run();
	virtual void End();
};

#endif//!_ACTION_RAND_COLOR_H