#include "Player.h"

Player::Player(const std::wstring& name, const std::wstring& id, Inventory& inventory, int level, int max_hp)
	: Actor(name, id, level, max_hp)
	, m_Inventory(inventory)
	, m_bActive{false}
	, m_PartyPosition{0}
{
}

Player::~Player()
{
}
