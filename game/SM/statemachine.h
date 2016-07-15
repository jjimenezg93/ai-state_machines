#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

class Character;
class State;

class StateMachine {
public:
	StateMachine(Character * owner);
	~StateMachine();

	void AddState(State * newState);

	void SetCurrentState(State * newCurrent);
	State * GetCurrentState() const;

	void Load();
	void Start();
	void Update();
private:
	std::vector<State *> m_states;
	State * m_currentState;

	Character * m_owner;
};

#endif//!_STATE_MACHINE_H