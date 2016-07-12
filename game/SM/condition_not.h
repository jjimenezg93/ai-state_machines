#ifndef _CONDITION_NOT_H
#define _CONDITION_NOT_H

#include "condition.h"

class ConditionNot: public Condition {
public:
	ConditionNot(Condition * oCondition);

	virtual bool Check() const;
private:
	Condition * m_originalCondition;
};

#endif//!_CONDITION_NOT_H