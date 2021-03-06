#include "stdafx.h"

#include "statemachine.h"
#include "state.h"
#include "action.h"
#include "transition.h"

StateMachine::StateMachine(Character * owner): m_owner(owner), m_currentState(nullptr) {}

StateMachine::~StateMachine() {
	std::vector<State *>::iterator delItr = m_states.begin();
	while (delItr != m_states.end()) {
		delete *delItr;
		delItr++;
	}
	m_states.clear();
}

//would be used to load state machine's from file
void StateMachine::Load() {}

void StateMachine::Start() {
	if (!m_currentState && !m_states.empty()) {
		m_currentState = m_states.at(0);
	}
}

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
