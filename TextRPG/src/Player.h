#pragma once

#include "Actor.h"
#include "Inventory.h"

class Player : public Actor
{
  private:
	Inventory& m_Inventory;
	bool m_bActive;
	int m_PartyPosition;

  public:
	Player(const std::wstring& name, const std::wstring& id, Inventory& inventory, int level, int max_hp);
	~Player();

	Inventory& GetInventory() { return m_Inventory; }
	void SetActive(bool active) { m_bActive = active; }
	const bool IsActive() const { return m_bActive; }
	void SetPartyPosition(int pos) { m_PartyPosition = pos; }
	const int GetPartyPosition() const { return m_PartyPosition; }
};