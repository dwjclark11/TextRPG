#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include "../Item.h"
#include "../Equipment.h"
#include "ShopParameters.h"

template <typename T>
void remove(std::vector<T>& vec, size_t index)
{
	vec.erase(vec.begin() + index);
}

std::wstring CharToWide(const char* str);
std::string WideToStr(const std::wstring& wstr);
std::wstring PadNumbers(int num);

Item::ItemType ItemTypeFromString(const std::string& item_type);
Equipment::EquipType EquipTypeFromString(const std::string& equip_type);
ShopParameters::ShopType ShopTypeFromString(const std::string& shopType);
WeaponProperties::WeaponType WeaponTypeFromString(const std::string& weapon_type);
ArmorProperties::ArmorType ArmorTypeFromString(const std::string& armor_type);
StatModifier::ModifierType ModifierTypeFromString(const std::string& mod_type);
StatModifier::ElementalType ElementalTypeFromString(const std::string& elemental_type);