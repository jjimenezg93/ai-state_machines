#ifndef _ACTION_ALARM_DELAY_H
#define _ACTION_ALARM_DELAY_H

#include "action.h"

class Character;

class ActionAlarmDelay: public Action {
public:
	ActionAlarmDelay(Character * character, double delay);

	virtual void Start();
	virtual void Run();
	virtual void End();
private:
	double m_desiredDelay;
	time_t m_startTime;

	Character * m_character;
};

#endif//!_ACTION_ALARM_DELAY_H