#pragma once
#include "Item.h"
#include "Equipment.h"
#include <vector>
#include <memory>

class Player;

class Inventory
{
private:
	std::vector<std::shared_ptr<Item>> m_Items;
	std::vector<std::shared_ptr<Equipment>> m_Equipment;

public:
	Inventory();
	~Inventory() = default;

	const std::vector<std::shared_ptr<Item>>& GetItems() { return m_Items; }
	const std::vector<std::shared_ptr<Equipment>>& GetEquipment() { return m_Equipment; }

	void AddItem(std::shared_ptr<Item> newItem);
	void AddEquipment(std::shared_ptr<Equipment> newEquipment);
	void UseItem(int index, Player& player);
};