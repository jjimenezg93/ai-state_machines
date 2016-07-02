#include "stdafx.h"

#include "transition.h"
#include "condition.h"
#include "action.h"
#include "state.h"

Transition::Transition(Condition * condition, State * targetState, Action * action):
m_condition(condition), m_targetState(targetState), m_triggerAction(action) {}

Transition::~Transition() {

}

bool Transition::CanTrigger() const {
	return m_condition->Check();
}

State * Transition::Trigger() {
	return new State();
}