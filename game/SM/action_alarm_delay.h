#ifndef _ACTION_ALARM_DELAY_H
#define _ACTION_ALARM_DELAY_H

#include "action.h"

class ActionAlarmDelay: public Action {
public:
	ActionAlarmDelay(float delay);

	virtual void Start();
	virtual void Run();
	virtual void End();
private:
	float m_currentDelay, m_desiredDelay;
	time_t m_startTime;
};

#endif//!_ACTION_ALARM_DELAY_H