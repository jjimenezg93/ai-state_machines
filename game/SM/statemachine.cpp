#include "stdafx.h"

#include "statemachine.h"
#include "state.h"
#include "action.h"
#include "transition.h"

StateMachine::StateMachine(Character * owner): m_owner(owner) {}

//would be used to load state machine's from file
void StateMachine::Load() {}

void StateMachine::Start() {}

void StateMachine::Update() {
	if (m_currentState) {
		m_currentState->Update();
		const std::vector<Transition *> & transitions = m_currentState->GetTransitions();
		for (std::vector<Transition *>::const_iterator cItr = transitions.begin();
		cItr != transitions.end(); ++cItr) {
			if ((*cItr)->CanTrigger()) {
				m_currentState->OnExit();
				m_currentState = (*cItr)->Trigger();
				m_currentState->OnEnter();
				return;
			}
		}
	}
}

void StateMachine::AddState(State * newState) {
	m_states.push_back(newState);
}

void StateMachine::SetCurrentState(State * newCurrent) {
	m_currentState = newCurrent;
}

State * StateMachine::GetCurrentState() const {
	return m_currentState;
}
