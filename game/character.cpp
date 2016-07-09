#include <stdafx.h>
#include <tinyxml.h>

#include "steering.h"
#include "aligntomovement_steering.h"
#include "seek_steering.h"
#include "character.h"
#include "params.h"
#include "pathfollowing_steering.h"
#include "pathfinding/pathfinder.h"
#include "obstacleavoidance_steering.h"
#include "obstaclepath_steering.h"

//State Machines
#include "SM/statemachine.h"
#include "SM/actionrandcolor.h"
#include "SM/actionnone.h"
#include "SM/state.h"
#include "SM/conditionmoving.h"
#include "SM/transition.h"

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

	mSteerings.push_back(new SeekSteering());
	for (std::vector<Steering*>::iterator it = mSteerings.begin(); it != mSteerings.end();
	++it) {
		(*it)->Init(this);
	}
	mTarget = mParams.target_position;
	mArriveRadius = mParams.arrive_radius;

	mStateMachine = new StateMachine(this);
	//memory leak, just testing
	State * defaultState = new State(new ActionRandColor(),
		new ActionRandColor(), new ActionRandColor());
	State * noneState = new State(new ActionNone(), new ActionNone(), new ActionNone());
	mStateMachine->AddState(defaultState);
	mStateMachine->SetCurrentState(defaultState);

	ConditionMoving * moving = new ConditionMoving(gIsMoving);
	Transition * transition = new Transition(moving, noneState, new ActionNone());
	mStateMachine->GetCurrentState()->AddTransition(transition);
	mStateMachine->Start();
	//SetImage(2);
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
	gIsMoving = !gIsMoving;
	mStateMachine->Update();
	SetImage(2);

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