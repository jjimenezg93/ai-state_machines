#include "stdafx.h"

#include "action_alarm_delay.h"
#include "character.h"

ActionAlarmDelay::ActionAlarmDelay(Character * character, float delay): m_character(character),
m_desiredDelay(delay), m_currentDelay(0.f) {}

void ActionAlarmDelay::Start() {
	m_startTime = std::time(nullptr);
}

void ActionAlarmDelay::Run() {
	time_t current = std::time(nullptr);
	if (abs(std::difftime(m_startTime, current)) >= m_desiredDelay) {
		m_character->SetCanMove(true);
	}
}

void ActionAlarmDelay::End() {}