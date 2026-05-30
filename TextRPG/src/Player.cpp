#include "actor/Player.h"
#include "items/Inventory.h"

Player::Player(Inventory& inventory, const PlayerProperties& props)
	: Actor(props.name, props.id, props.level, props.maxHP, props.type)
	, m_Inventory(inventory)
	, m_bActive{ false }
	, m_PartyPosition{ 0 }
{

}

Player::Player(const Player& other)
	: Player(other.m_Inventory, PlayerProperties{ other.m_sName, other.m_sID, other.m_MaxHP, other.m_Level, other.m_eActorType })
{

}

Player::~Player()
{
}
