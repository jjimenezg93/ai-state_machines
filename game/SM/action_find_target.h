#ifndef _ACTION_FIND_TARGET_H
#define _ACTION_FIND_TARGET_H

#include "action.h"

class Character;

class ActionFindTarget: public Action {
public:
	ActionFindTarget(Character * character);

	virtual void Start();
	virtual void Run();
	virtual void End();
private:
	Character * m_character;
};

#endif//!_ACTION_FIND_TARGET_H