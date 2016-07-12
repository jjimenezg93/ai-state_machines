#include "stdafx.h"

#include "condition_found_target.h"
#include "character.h"

ConditionFoundTarget::ConditionFoundTarget(Character * character): m_character(character) {}

bool ConditionFoundTarget::Check() const {
	return m_character->IsTargetActive();
}