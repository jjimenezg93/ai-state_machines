#include "stdafx.h"
#include "character.h"
#include "arrive_steering.h"


ArriveSteering::ArriveSteering() {
}


ArriveSteering::~ArriveSteering() {
}

void ArriveSteering::Init(Character * ch) {
	Steering::Init(ch);
}

void ArriveSteering::Update(Accelerations &acc, USVec2D target) {
	if (GetCh()->CanMove()) {//added for SM's
		Character * ch = GetCh();
		USVec2D chLoc = ch->GetLoc();
		USVec2D desiredVelocity = target - chLoc;
		float arriveFactor = desiredVelocity.Length();
		if (arriveFactor < ch->GetArriveRadius()) {
			desiredVelocity.Norm();
			desiredVelocity = desiredVelocity *
				ch->GetParams().max_velocity * (arriveFactor / ch->GetArriveRadius());
			USVec2D wishAcc = desiredVelocity - ch->GetLinearVelocity();
			float accFactor = wishAcc.Norm();
			acc.linearAcc = wishAcc * accFactor * ch->GetParams().max_acceleration;
		} else {
			USVec2D wishAcc = desiredVelocity - ch->GetLinearVelocity();
			float accFactor = wishAcc.Norm();
			acc.linearAcc = wishAcc * accFactor * ch->GetParams().max_acceleration;
		}
		mLastLinearAcc = acc.linearAcc;
	}
}

void ArriveSteering::DrawDebug() {
	Character * ch = GetCh();
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);

	//LinearAcc
	/*gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(static_cast<USVec2D>(ch->GetLoc()),
		static_cast<USVec2D>(ch->GetLoc()) + mLastLinearAcc);

	gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);

	MOAIDraw::DrawPoint(ch->GetParams().target_position);
	MOAIDraw::DrawEllipseOutline(ch->GetParams().target_position.mX,
		ch->GetParams().target_position.mY, ch->GetParams().arrive_radius,
		ch->GetParams().arrive_radius, 64);*/
}