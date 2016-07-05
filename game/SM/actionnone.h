#ifndef _ACTION_NONE_H
#define _ACTION_NONE_H

#include "action.h"

class ActionNone: public Action {
public:
	virtual void Start();
	virtual void Run();
	virtual void End();
};

#endif//!_ACTION_NONE_H