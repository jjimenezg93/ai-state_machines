#ifndef _ACTION_CHANGE_IMAGE_H
#define _ACTION_CHANGE_IMAGE_H

#include "action.h"

class ActionChangeImage: public Action {
public:
	ActionChangeImage(unsigned char imageID);

	virtual void Start(Character * character);
	virtual void Run();
	virtual void End();
private:
	unsigned char m_targetImage;
};

#endif