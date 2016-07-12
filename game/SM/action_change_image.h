#ifndef _ACTION_CHANGE_IMAGE_H
#define _ACTION_CHANGE_IMAGE_H

#include "action.h"

class Character;

class ActionChangeImage: public Action {
public:
	ActionChangeImage(Character * character, unsigned char imageID);

	virtual void Start();
	virtual void Run();
	virtual void End();
private:
	unsigned char m_targetImage;
	Character * m_character;
};

#endif