#ifndef _TRANSITION_H
#define _TRANSITION_H

class Condition;
class State;
class Action;

class Transition {
public:
	Transition(Condition * condition, State * targetState, Action * action);
	~Transition();

	bool CanTrigger() const;
	State * Trigger();
private:
	Condition * m_condition;
	State * m_targetState;
	Action * m_triggerAction;
};

#endif//!_TRANSITION_H