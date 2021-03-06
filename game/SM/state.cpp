#include "stdafx.h"

#include "state.h"
#include "transition.h"
#include "action.h"

State::State() {}

State::State(Action * enter, Action * update, Action * exit):
	m_enterAction(enter), m_updateAction(update), m_exitAction(exit) {

}

State::~State() {
	std::vector<Transition *>::iterator delItr = m_transitions.begin();
	while (delItr != m_transitions.end()) {
		delete *delItr;
		delItr++;
	}
	m_transitions.clear();
}

void State::OnEnter() {
	if (m_enterAction)
		m_enterAction->Start();
	if (m_updateAction)
		m_updateAction->Start();
}

void State::Update() {
	if (m_updateAction)
		m_updateAction->Run();
}

void State::OnExit() {
	if (m_exitAction)
		m_exitAction->End();
}

const std::vector<Transition *> & State::GetTransitions() const {
	return m_transitions;
}

void State::AddTransition(Transition * newTransition) {
	m_transitions.push_back(newTransition);
}

void State::SetEnterAction(Action * enter) {
	m_enterAction = enter;
}

void State::SetUpdateAction(Action * update) {
	m_updateAction = update;
}

void State::SetExitAction(Action * exit) {
	m_exitAction = exit;
}
