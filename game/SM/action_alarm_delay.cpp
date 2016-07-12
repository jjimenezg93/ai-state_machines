#include "stdafx.h"

#include "action_alarm_delay.h"

ActionAlarmDelay::ActionAlarmDelay(float delay): m_desiredDelay(delay), m_currentDelay(0.f) {}

void ActionAlarmDelay::Start() {
	m_startTime = std::time(nullptr);
}

void ActionAlarmDelay::Run() {
	if (std::difftime(m_startTime, std::time(nullptr)) >= m_desiredDelay) {
		
	}
}

void ActionAlarmDelay::End() {}