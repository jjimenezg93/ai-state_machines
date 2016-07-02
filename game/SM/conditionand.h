#ifndef _CONDITION_AND_H
#define _CONDITION_AND_H

#include "condition.h"

class ConditionAnd: public Condition {
public:
	ConditionAnd(Condition * first, Condition * second);

	virtual bool Check() const;
private:
	Condition * m_firstCondition;
	Condition * m_secondCondition;
};

#endif//!_CONDITION_AND_H