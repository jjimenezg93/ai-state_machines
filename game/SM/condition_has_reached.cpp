#include "stdafx.h"

#include "character.h"
#include "condition_has_reached.h"

#define ARRIVE_EPSILON 15.f

ConditionHasReached::ConditionHasReached(Character * character, USVec2D &position):
m_character(character), m_targetPosition(position) {}

bool ConditionHasReached::Check() const {
	USVec2D currPos = m_character->GetLoc();
	if ((currPos - m_targetPosition).Length() <= ARRIVE_EPSILON) {
		m_character->SetCanMove(false);
		m_character->SetIsTargetActive(false);//should it be here or in another action/trans?
		m_character->SetLinearVelocity(0.f, 0.f);//hotfix. This shouldn't be done here
		return true;
	}
	return false;
}

