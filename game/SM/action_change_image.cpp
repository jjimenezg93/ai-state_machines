#include "stdafx.h"
#include "character.h"
#include "action_change_image.h"

ActionChangeImage::ActionChangeImage(Character * character, unsigned char imageID):
	m_character(character), m_targetImage(imageID) {}

void ActionChangeImage::Start() {
	m_character->SetImage(m_targetImage);
}

void ActionChangeImage::Run() {}

void ActionChangeImage::End() {}
