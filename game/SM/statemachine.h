#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

class State;

class StateMachine {
public:
	void AddState(State * newState);

	void Start();
	void Update();
private:
	std::vector<State *> m_states;
	State * m_currentState;
};

#endif//!_STATE_MACHINE_H