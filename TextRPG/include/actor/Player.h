#pragma once

#include "Actor.h"

class Inventory;

struct PlayerProperties
{
	std::wstring name{ L"player" };
	std::wstring id{ L"id" };
	int level{ 1 };
	int maxHP{ 100 };
	Actor::ActorType type{ Actor::ActorType::WARRIOR };
};

class Player : public Actor
{
  
  public:
	Player(Inventory& inventory, const PlayerProperties& props);
	Player(const Player& other);

	~Player();

	Inventory& GetInventory() { return m_Inventory; }
	void SetActive(bool active) { m_bActive = active; }
	const bool IsActive() const { return m_bActive; }
	void SetPartyPosition(int pos) { m_PartyPosition = pos; }
	const int GetPartyPosition() const { return m_PartyPosition; }

private:
	Inventory& m_Inventory;
	bool m_bActive;
	int m_PartyPosition;
};
