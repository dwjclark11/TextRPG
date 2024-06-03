#include "Potion.h"
#include "Player.h"

Potion::Potion(const std::wstring& item_name, const std::wstring& desc, int health, int buy_price)
{
	m_sItemName = item_name;
	m_sDescription = desc;
	m_ItemValue = health;
	m_BuyPrice = buy_price;
	m_SellPrice = buy_price / 2;
	SetType(ItemType::HEALTH);
}

Potion::~Potion()
{
}

bool Potion::OnUse(Player& player)
{
	if (m_Count <= 0)
		return false;

	const auto& hp = player.GetHP();
	const auto& hp_max = player.GetMaxHP();

	if (hp >= hp_max)
		return false;

	player.HealHP(m_ItemValue);

	Decrement();

	return true;
}
