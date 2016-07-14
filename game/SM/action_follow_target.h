#ifndef _ACTION_FOLLOW_TARGET_H
#define _ACTION_FOLLOW_TARGET_H

#include "action.h"

class Character;

class ActionFollowTarget: public Action {
public:
	ActionFollowTarget(Character * character);

	virtual void Start();
	virtual void Run();
	virtual void End();
private:

	Character * m_character;
};

#endif//!_ACTION_FOLLOW_TARGET_H