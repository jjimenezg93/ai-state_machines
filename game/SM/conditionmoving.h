#ifndef _CONDITION_MOVING_H
#define _CONDITION_MOVING_H

#include "condition.h"

class ConditionMoving: public Condition {
public:
	ConditionMoving(bool &isMoving);

	virtual bool Check() const;
private:
	bool &m_IsMoving;
};

#endif//!_CONDITION_MOVING_H