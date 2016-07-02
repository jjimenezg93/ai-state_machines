#ifndef _STATE_H
#define _STATE_H

#include <vector>

class Action;
class Transition;
#include "action.h"
#include "transition.h"

class State {
public:
	State();
	//State(Action * enter, Action * update, Action * exit);

	void OnEnter();
	void Update();
	void OnExit();

	const std::vector<Transition *> & GetTransitions() const;
	void AddTransition(Transition * newTransition);

	void SetEnterAction(Action * enter);
	void SetUpdateAction(Action * update);
	void SetExitAction(Action * exit);
private:
	Action * m_enterAction;
	Action * m_updateAction;
	Action * m_exitAction;
	std::vector<Transition *> m_transitions;
};

#endif//!_STATE_H