#include "stdafx.h"

#include "statemachine.h"
#include "state.h"
#include "action.h"

void StateMachine::AddState(State * newState) {
	m_states.push_back(newState);
}

void StateMachine::Update() {
	if (m_currentState) {
		m_currentState->Update();
	}
}
