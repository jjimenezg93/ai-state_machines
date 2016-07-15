#include <stdafx.h>
#include <tinyxml.h>

#include "steering.h"
#include "aligntomovement_steering.h"
#include "arrive_steering.h"
#include "seek_steering.h"
#include "character.h"
#include "params.h"
#include "pathfollowing_steering.h"
#include "pathfinding/pathfinder.h"
#include "obstacleavoidance_steering.h"
#include "obstaclepath_steering.h"

//State Machines
#include "SM/action_alarm_delay.h"
#include "SM/action_change_image.h"
#include "SM/action_none.h"
#include "SM/action_find_target.h"
#include "SM/action_follow_target.h"
#include "SM/condition_and.h"
#include "SM/condition_can_move.h"
#include "SM/condition_found_target.h"
#include "SM/condition_has_reached.h"
#include "SM/condition_not.h"
#include "SM/statemachine.h"
#include "SM/state.h"
#include "SM/transition.h"

#define INPUT_MIN_TIME 0.2f

std::vector<Action *> actions;
std::vector<Condition *> conditions;
//actions and conditions are not freed inside SM's, states or transitions because
//they are more likely to be shared by some of them and even between multiple SM's than the rest


Character::Character(): mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f),
mStateMachine(nullptr) {
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END
}

Character::~Character() {
	if (mStateMachine != nullptr) {
		delete mStateMachine;

		std::vector<Action *>::iterator delActItr = actions.begin();
		while (delActItr != actions.end()) {
			delete *delActItr;
			delActItr++;
		}
		actions.clear();

		std::vector<Condition *>::iterator delCondItr = conditions.begin();
		while (delCondItr != conditions.end()) {
			delete *delCondItr;
			delCondItr++;
		}
		conditions.clear();
	}
}

void Character::OnStart() {
	ReadParams("params.xml", mParams);

	mSteerings.push_back(new ArriveSteering());
	mSteerings.push_back(new AlignToMovementSteering());
	for (std::vector<Steering*>::iterator it = mSteerings.begin(); it != mSteerings.end();
	++it) {
		(*it)->Init(this);
	}
	mTarget = mParams.target_position;
	mArriveRadius = mParams.arrive_radius;

	mLastInputTime = 0.f;
	mCanMove = false;
	mIsTargetActive = false;

	mStateMachine = new StateMachine(this);

	//Actions
	Action * aNone = new ActionNone();
	actions.push_back(aNone);
	Action * aChgImgToIdle = new ActionChangeImage(this, 0);
	actions.push_back(aChgImgToIdle);
	Action * aChgImgToAlarm = new ActionChangeImage(this, 1);
	actions.push_back(aChgImgToAlarm);
	Action * aChgImgToFollowing = new ActionChangeImage(this, 2);
	actions.push_back(aChgImgToFollowing);
	Action * aFindTarget = new ActionFindTarget(this);
	actions.push_back(aFindTarget);
	Action * aAlarmDelay = new ActionAlarmDelay(this, 2.f);
	actions.push_back(aAlarmDelay);
	Action * aFollowTarget = new ActionFollowTarget(this);
	actions.push_back(aFollowTarget);

	//States
	State * sIdle = new State(aChgImgToIdle, aFindTarget, aNone);
	State * sAlarm = new State(aChgImgToAlarm, aAlarmDelay, aNone);
	State * sFollowing = new State(aChgImgToFollowing, aFollowTarget, aNone);

	//Conditions
	Condition * cFoundTarget = new ConditionFoundTarget(this);
	conditions.push_back(cFoundTarget);
	Condition * cNotFoundTarget = new ConditionNot(cFoundTarget);//should add some kind of return
	//selection, so we could use cFoundTarget to enable&disable alarm state
	conditions.push_back(cNotFoundTarget);
	Condition * cHasReached = new ConditionHasReached(this, mTarget);
	conditions.push_back(cHasReached);
	Condition * cCanMove = new ConditionCanMove(this);
	conditions.push_back(cCanMove);

	//Transitions
	Transition * tIdleAlarm = new Transition(cFoundTarget, sAlarm, aNone);
	Transition * tAlarmIdle = new Transition(cHasReached, sIdle, aNone);
	Transition * tAlarmFollow = new Transition(cCanMove, sFollowing, aNone);
	Transition * tFollowIdle = new Transition(cHasReached, sIdle, aNone);

	//SM initialization
	sIdle->AddTransition(tIdleAlarm);
	sAlarm->AddTransition(tAlarmIdle);
	sAlarm->AddTransition(tAlarmFollow);
	sFollowing->AddTransition(tFollowIdle);

	mStateMachine->AddState(sIdle);
	mStateMachine->AddState(sAlarm);
	mStateMachine->AddState(sFollowing);

	mStateMachine->SetCurrentState(sIdle);

	mStateMachine->Start();
}

void Character::FillPath() {
	mParams.points.clear();
	mParams.numSegments = 0;
	const std::vector<USVec2D> * path = &(Pathfinder::Instance().GetPath());
	uint16_t i = 0;
	while (i < path->size()) {
		mParams.points.push_back(Point(path->at(i).mX, path->at(i).mY));
		mParams.numSegments++;
		i++;
	}
}

void Character::OnStop() {

}

void Character::OnUpdate(float step) {
	mLastInputTime += step;

	mStateMachine->Update();

	Accelerations acc;
	for (std::vector<Steering *>::iterator itr = mSteerings.begin(); itr != mSteerings.end();
	++itr) {
		(*itr)->Update(acc, mTarget);
	}

	mLastLinearAcc = acc.linearAcc;
	mLinearVelocity.mX += acc.linearAcc.mX * step;
	mLinearVelocity.mY += acc.linearAcc.mY * step;

	if (mLinearVelocity.mX > mParams.max_velocity)
		mLinearVelocity.mX = mParams.max_velocity;
	if (mLinearVelocity.mX < -mParams.max_velocity)
		mLinearVelocity.mX = -mParams.max_velocity;
	if (mLinearVelocity.mY > mParams.max_velocity)
		mLinearVelocity.mY = mParams.max_velocity;
	if (mLinearVelocity.mY < -mParams.max_velocity)
		mLinearVelocity.mY = -mParams.max_velocity;

	mAngularVelocity += acc.angularAcc * step;

	if (mAngularVelocity > mParams.max_angular_velocity) {
		mAngularVelocity = mParams.max_angular_velocity;
	} else if (mAngularVelocity < -mParams.max_angular_velocity) {
		mAngularVelocity = -mParams.max_angular_velocity;
	}

	SetLoc(GetLoc() + static_cast<USVec2D>(mLinearVelocity) * step);
	SetRot(GetRot() + mAngularVelocity * step);
}

void Character::DrawDebug() {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);

	for (std::vector<Steering *>::iterator itr = mSteerings.begin(); itr != mSteerings.end();
	++itr) {
		(*itr)->DrawDebug();
	}

	//LinearVel
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(static_cast<USVec2D>(GetLoc()),
		static_cast<USVec2D>(GetLoc()) + mLinearVelocity);

	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(static_cast<USVec2D>(GetLoc()),
		static_cast<USVec2D>(GetLoc()) + mLastLinearAcc);

	gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
	MOAIDraw::DrawPoint(mTarget.mX, mTarget.mY);
}

// Lua configuration
void Character::RegisterLuaFuncs(MOAILuaState& state) {
	MOAIEntity2D::RegisterLuaFuncs(state);

	luaL_Reg regTable[] = {
		{"setLinearVel", _setLinearVel},
		{"setAngularVel", _setAngularVel},
		{"setTarget", _setTarget},
		{"setTargetActive", _setTargetActive},
		{NULL, NULL}
	};

	luaL_register(state, 0, regTable);
}

int Character::_setLinearVel(lua_State* L) {
	MOAI_LUA_SETUP(Character, "U")

		float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetLinearVelocity(pX, pY);
	return 0;
}

int Character::_setAngularVel(lua_State* L) {
	MOAI_LUA_SETUP(Character, "U")

		float angle = state.GetValue<float>(2, 0.0f);
	self->SetAngularVelocity(angle);

	return 0;
}

int Character::_setTarget(lua_State* L) {
	MOAI_LUA_SETUP(Character, "U")

		self->mTarget = USVec2D(state.GetValue<float>(2, 0.0f), state.GetValue<float>(3, 0.0f));

	return 0;
}

int Character::_setTargetActive(lua_State* L) {
	MOAI_LUA_SETUP(Character, "U")

		if (self->mLastInputTime >= INPUT_MIN_TIME) {
			self->SetIsTargetActive(!self->mIsTargetActive);
			self->mLastInputTime = 0.f;
		}

	return 0;
}
