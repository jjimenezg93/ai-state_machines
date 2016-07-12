#include "stdafx.h"
#include "condition_not.h"

ConditionNot::ConditionNot(Condition * oCondition):
	m_originalCondition(oCondition) {
}

bool ConditionNot::Check() const {
	return !m_originalCondition->Check();
}