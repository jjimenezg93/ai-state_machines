#ifndef _CONDITION_FOUND_TARGET_H
#define _CONDITION_FOUND_TARGET_H

#include "condition.h"

class Character;

class ConditionFoundTarget: public Condition {
public:
	ConditionFoundTarget(Character * character);

	virtual bool Check() const;
private:
	Character * m_character;
};

#endif//!_CONDITION_FOUND_TARGET_H