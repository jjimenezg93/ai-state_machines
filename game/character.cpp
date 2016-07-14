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
#include "SM/condition_and.h"
#include "SM/condition_found_target.h"
#include "SM/condition_has_reached.h"
#include "SM/condition_not.h"
#include "SM/statemachine.h"
#include "SM/state.h"
#include "SM/transition.h"

#define INPUT_MIN_TIME 0.2f

bool gIsMoving = false;

Character::Character(): mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f) {
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END
}

Character::~Character() {

}

void Character::OnStart() {
	ReadParams("params.xml", mParams);

	//mSteerings.push_back(new SeekSteering());
	mSteerings.push_back(new ArriveSteering());
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
	State * idleState = new State();
	/* DEALLOC ALL MEMORY OF SM, STATES, ACTIONS, CONDITIONS AND TRANSITIONS*/
	//Actions
	Action * aNone = new ActionNone();
	Action * aChgImgToIdle = new ActionChangeImage(this, 0);
	Action * aChgImgToAlarm = new ActionChangeImage(this, 1);
	Action * aChgImgToWindUp = new ActionChangeImage(this, 2);
	Action * aFindTarget = new ActionFindTarget(this);
	Action * aAlarmDelay = new ActionAlarmDelay(this, 2.f);

	//States
	State * sIdle = new State(aChgImgToIdle, aFindTarget, aNone);
	State * sAlarm = new State(aChgImgToAlarm, aAlarmDelay, aNone);
	//State * sWindup = new State(aChgImgToWindUp, aFindTarget, aNone);

	//Conditions
	Condition * cFoundTarget = new ConditionFoundTarget(this);
	Condition * cNotFoundTarget = new ConditionNot(cFoundTarget);//should add some kind of return
	//selection, so we could use cFoundTarget to enable&disable alarm state
	Condition * cHasReached = new ConditionHasReached(this, mTarget);

	//Transitions
	Transition * tIdleAlarm = new Transition(cFoundTarget, sAlarm, aNone);
	Transition * tAlarmIdle = new Transition(cHasReached, sIdle, aNone);
	
	//SM initialization
	sIdle->AddTransition(tIdleAlarm);
	sAlarm->AddTransition(tAlarmIdle);

	mStateMachine->AddState(sIdle);
	mStateMachine->AddState(sAlarm);
	
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

	gIsMoving = !gIsMoving;
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
