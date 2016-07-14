#ifndef _CONDITION_HAS_REACHED_H
#define _CONDITION_HAS_REACHED_H

#include "condition.h"
class Character;

class ConditionHasReached: public Condition {
public:
	ConditionHasReached(Character * character, USVec2D &position);

	virtual bool Check() const;
private:
	Character * m_character;
	USVec2D &m_targetPosition;
};

#endif//!_CONDITION_HAS_REACHED_H