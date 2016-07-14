#ifndef _ARRIVE_STEERING_H_
#define _ARRIVE_STEERING_H_

#include "steering.h"

class ArriveSteering: public Steering {
public:
	ArriveSteering();
	virtual ~ArriveSteering();

	virtual void Init(Character * ch);

	virtual void DrawDebug();

	virtual void Update(Accelerations &acc, USVec2D target);
private:
	USVec2D mLastLinearAcc;
};

#endif //!_ARRIVE_STEERING_H_