#include "stdafx.h"
#include "character.h"
#include "actionchangeimage.h"

ActionChangeImage::ActionChangeImage(unsigned char imageID): m_targetImage(imageID) {}

void ActionChangeImage::Start(Character * character) {
	character->SetImage(m_targetImage);
}

void ActionChangeImage::Run() {}

void ActionChangeImage::End() {}
