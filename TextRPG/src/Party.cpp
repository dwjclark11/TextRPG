#include "Party.h"
#include "Player.h"
#include "Logger.h"
#include "utilities/trpg_utilities.h"

Party::Party()
	: m_PartyMembers{}
	, m_Inventory{}
	, m_Gold{500}
	, m_NumActiveMembers{0}
{
}

Party::~Party()
{
}

bool Party::AddMember(std::shared_ptr<Player> newMember)
{
	if (m_PartyMembers.size() >= MAX_MEMBERS)
	{
		TRPG_ERROR("There are already [" + std::to_string(MAX_MEMBERS) + "] members in the Party!\nPlease remove one!");
		return false;
	}

	// Check to see if the member is already in the party
	for (const auto& member : m_PartyMembers)
	{
		if (member->GetID() == newMember->GetID())
		{
			TRPG_ERROR("Member with that ID already exists in the party!");
			return false;
		}
	}

	if (m_NumActiveMembers < MAX_ACTIVE_MEMBERS)
	{
		newMember->SetActive(true);
		m_NumActiveMembers++;
		newMember->SetPartyPosition(m_NumActiveMembers);
	}

	m_PartyMembers.push_back(std::move(newMember));

	return true;
}

bool Party::RemoveMember(const std::wstring& memberID, bool eliminate)
{
	for (int i = 0; i < m_PartyMembers.size(); i++)
	{
		auto& member = m_PartyMembers[ i ];
		if (member->GetID() != memberID)
			continue;

		member->SetActive(false);
		m_NumActiveMembers--;

		if (eliminate)
			remove(m_PartyMembers, i);

		return true;
	}

	TRPG_ERROR("Failed to remove the Party Member!");
	return false;
}

void Party::AddGold(int gold)
{
	m_Gold += gold;

	if (m_Gold >= MAX_GOLD)
		m_Gold = MAX_GOLD;
}

bool Party::BuyItem(int price, std::shared_ptr<Item> item)
{
	if (m_Gold < price)
		return false;

	if (!m_Inventory.AddItem(std::move(item)))
	{
		TRPG_ERROR("Failed to add the item to the inventory!");
		return false;
	}
	m_Gold -= price;

	if (m_Gold <= 0)
		m_Gold = 0;

	return true;
}

bool Party::BuyEquipment(int price, std::shared_ptr<Equipment> equipment)
{
	if (m_Gold < price)
		return false;

	if (!m_Inventory.AddEquipment(std::move(equipment)))
	{
		TRPG_ERROR("Failed to add the equipment to the inventory!");
		return false;
	}

	m_Gold -= price;

	if (m_Gold <= 0)
		m_Gold = 0;

	return true;
}
