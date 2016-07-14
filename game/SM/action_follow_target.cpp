#include "stdafx.h"

#include "action_follow_target.h"

ActionFollowTarget::ActionFollowTarget(Character * character): m_character(character) {}

void ActionFollowTarget::Start() {}

void ActionFollowTarget::Run() {}//this actually does nothing since movement is handled by
//ArriveSteering

void ActionFollowTarget::End() {}
