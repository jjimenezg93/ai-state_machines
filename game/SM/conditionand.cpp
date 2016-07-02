#include "stdafx.h"
#include "conditionand.h"

ConditionAnd::ConditionAnd(Condition * first, Condition * second):
m_firstCondition(first), m_secondCondition(second) {}

bool ConditionAnd::Check() const {
	return m_firstCondition->Check() && m_secondCondition->Check();
}