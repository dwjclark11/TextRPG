#pragma once
#include "Item.h"

class Player;

class Potion : public Item
{
public:
	Potion();
	Potion(const std::wstring& item_name, const std::wstring& desc, int health = 10, int buy_price = 0);
	~Potion();
	bool OnUse(Player& player) override;
};
