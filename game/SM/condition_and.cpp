#include "stdafx.h"
#include "condition_and.h"

ConditionAnd::ConditionAnd(Condition * first, Condition * second):
m_firstCondition(first), m_secondCondition(second) {}

bool ConditionAnd::Check() const {
	return m_firstCondition->Check() && m_secondCondition->Check();
}