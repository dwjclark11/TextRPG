#include "Inventory.h"
#include "Logger.h"
#include "utilities/trpg_utilities.h"

Inventory::Inventory()
{
}

bool Inventory::AddItem(std::shared_ptr<Item> newItem)
{
	for (auto& item : m_Items)
	{
		if (item->GetItemName() == newItem->GetItemName())
			return item->AddItem(newItem->GetCount());
	}

	m_Items.push_back(std::move(newItem));

	return true;
}

bool Inventory::AddEquipment(std::shared_ptr<Equipment> newEquipment)
{
	for (auto& equip : m_Equipment)
	{
		if (equip->GetName() == newEquipment->GetName())
			return equip->Add(newEquipment->GetCount());
	}

	m_Equipment.push_back(std::move(newEquipment));

	return true;
}

bool Inventory::UseItem(int index, Player& player)
{
	if (m_Items.empty())
	{
		TRPG_ERROR("Failed to use items! Items are empty!");
		return false;
	}

	if (index >= m_Items.size())
	{
		TRPG_ERROR("Failed to use item. Index is beyond item vector size - INDEX[" + std::to_string(index) + "]");
		return false;
	}

	m_Items[ index ]->OnUse(player);

	if (m_Items[ index ]->GetCount() <= 0)
		remove(m_Items, index);

	return true;
}
