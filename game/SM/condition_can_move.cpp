#include "stdafx.h"

#include "character.h"
#include "condition_can_move.h"

ConditionCanMove::ConditionCanMove(Character * character): m_character(character) {}

bool ConditionCanMove::Check() const {
	return m_character->CanMove();
}