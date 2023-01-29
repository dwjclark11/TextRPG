#include "Inventory.h"

Inventory::Inventory()
{

}

bool Inventory::AddItem(std::shared_ptr<Item> newItem)
{
	return false;
}

bool Inventory::AddEquipment(std::shared_ptr<Equipment> newEquipment)
{
	return false;
}

bool Inventory::UseItem(int index, Player& player)
{
	return false;
}
