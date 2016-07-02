#ifndef _ACTION_H
#define _ACTION_H

class Action {
public:
	virtual void Start() {}
	virtual void Run() {}
	virtual void End() {}
};

#endif//!_ACTION_H