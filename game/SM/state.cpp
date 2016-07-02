#include "stdafx.h"

#include "state.h"
#include "transition.h"
#include "action.h"

State::State() {}

State::State(Action * enter, Action * update, Action * exit):
m_enterAction(enter), m_updateAction(update), m_exitAction(exit) {

}

void State::OnEnter() {
	m_enterAction->Run();
}

void State::Update() {
	m_updateAction->Run();
}

void State::OnExit() {
	m_exitAction->Run();
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
