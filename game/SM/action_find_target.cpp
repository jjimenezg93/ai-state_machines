#include "stdafx.h"
#include "action_find_target.h"
#include "character.h"

ActionFindTarget::ActionFindTarget(Character * character): m_character(character) {}

void ActionFindTarget::Start() {}

void ActionFindTarget::Run() {
	//in a more complex SM, here we could be doing raycasting or whatever it uses to find the
	//target, and it could call m_character->SetIsTargetActive(true); when found it, the proper
	//condition would change the state as it does now
}

void ActionFindTarget::End() {}
