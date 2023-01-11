#pragma once

#include "Actor.h"

class Player : public Actor
{
private:

public:
	Player(const std::wstring& name, int level, int max_hp);
	~Player();


};