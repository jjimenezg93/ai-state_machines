#ifndef _ACTION_FIND_TARGET_H
#define _ACTION_FIND_TARGET_H

#include "action.h"
#include "character.h"

class ActionFindTarget: public Action {
public:
	ActionFindTarget(Character * character);

	virtual void Start();
	virtual void Run();
	virtual void End();
private:
	Character * m_target;
};

#endif//!_ACTION_FIND_TARGET_H