#ifndef _CONDITION_CAN_MOVE_H
#define _CONDITION_CAN_MOVE_H

#include "condition.h"

class Character;

class ConditionCanMove: public Condition {
public:
	ConditionCanMove(Character * character);

	virtual bool Check() const;
private:
	Character * m_character;
};

#endif//!_CONDITION_CAN_MOVE_H