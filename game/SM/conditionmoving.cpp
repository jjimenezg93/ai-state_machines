#include "stdafx.h"
#include "conditionmoving.h"

ConditionMoving::ConditionMoving(bool &isMoving): m_IsMoving(isMoving) {}

bool ConditionMoving::Check() const {
	return m_IsMoving;
}