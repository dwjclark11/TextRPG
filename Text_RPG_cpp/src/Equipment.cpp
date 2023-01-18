#include "Equipment.h"

Weapon::Weapon()
	: Weapon(L"Weapon_Name", L"Weapon description", 0, 0)
{

}

Weapon::Weapon(const std::wstring& name, const std::wstring& description, int buy_price, int sell_price, WeaponProperties weapon_properties, StatModifier stat_modifier)
{
	m_sName = name;
	m_sDescription = description;
	m_BuyPrice = buy_price;
	m_SellPrice = sell_price;
	m_StatModifier = stat_modifier;
	SetEquipType(EquipType::WEAPON);
	m_WeaponProperties = weapon_properties;
	m_ArmorProperties = ArmorProperties();
}

bool Weapon::OnEquip(Player& player)
{
	return false;
}

bool Weapon::OnRemove(Player& player)
{
	return false;
}

Armor::Armor()
	: Armor(L"Armor_name", L"Armor Description", 0, 0)
{

}

Armor::Armor(const std::wstring& name, const std::wstring& description, int buy_price, int sell_price, ArmorProperties armor_properties, StatModifier stat_modifier)
{
	m_sName = name;
	m_sDescription = description;
	m_BuyPrice = buy_price;
	m_SellPrice = sell_price;
	m_StatModifier = stat_modifier;
	SetEquipType(EquipType::ARMOR);
	m_WeaponProperties = WeaponProperties();
	m_ArmorProperties = armor_properties;
}

bool Armor::OnEquip(Player& player)
{
	return false;
}

bool Armor::OnRemove(Player& player)
{
	return false;
}
