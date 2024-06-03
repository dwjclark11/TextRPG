#include "EquipmentLoader.h"
#include "trpg_utilities.h"
#include "ItemCreator.h"
#include "../Logger.h"
#include <cassert>

using namespace tinyxml2;

WeaponProperties EquipmentLoader::CreateWeaponProperties(tinyxml2::XMLElement* xmlElement)
{
	XMLElement* pWeaponProps = xmlElement->FirstChildElement("WeaponProperties");
	if (!pWeaponProps)
		return WeaponProperties();

	// Get the attack pwr
	XMLElement* pAttackPwr = pWeaponProps->FirstChildElement("AttackPwr");
	int attackPwr = pAttackPwr ? atoi(pAttackPwr->GetText()) : 0;

	// Get the weapon type
	XMLElement* pWeaponType = pWeaponProps->FirstChildElement("WeaponType");
	const std::string weapon_type_str = pWeaponType ? std::string{pWeaponType->GetText()} : "";
	WeaponProperties::WeaponType type = WeaponTypeFromString(weapon_type_str);

	return WeaponProperties(attackPwr, type);
}

ArmorProperties EquipmentLoader::CreateArmorProperties(tinyxml2::XMLElement* xmlElement)
{
	XMLElement* pArmorProps = xmlElement->FirstChildElement("ArmorProperties");
	if (!pArmorProps)
		return ArmorProperties();

	// Get the defense power
	XMLElement* pDefensePwr = pArmorProps->FirstChildElement("DefensePwr");
	int defense_pwr = pDefensePwr ? atoi(pDefensePwr->GetText()) : 0;

	// Get the armor type
	XMLElement* pArmorType = pArmorProps->FirstChildElement("ArmorType");
	const std::string armor_type_str = pArmorType ? std::string{pArmorType->GetText()} : "";
	ArmorProperties::ArmorType type = ArmorTypeFromString(armor_type_str);

	return ArmorProperties(defense_pwr, type);
}

StatModifier EquipmentLoader::CreateStatModifier(tinyxml2::XMLElement* xmlElement)
{
	XMLElement* pStatModifier = xmlElement->FirstChildElement("StatModifier");

	// If there is no stat mod, return an empty
	if (!pStatModifier)
		return StatModifier();

	// Get the modifier value
	XMLElement* pModifierVal = pStatModifier->FirstChildElement("ModValue");
	int mod_value = pModifierVal ? atoi(pModifierVal->GetText()) : 0;

	// Get the modifier type
	XMLElement* pModifierType = pStatModifier->FirstChildElement("ModType");
	const std::string mod_type_str = pModifierType ? std::string{pModifierType->GetText()} : "";
	StatModifier::ModifierType mod_type = ModifierTypeFromString(mod_type_str);

	// Get the elemental type
	XMLElement* pElementalType = pStatModifier->FirstChildElement("ElementalType");
	const std::string elemental_type_str = pElementalType ? std::string{pElementalType->GetText()} : "";
	StatModifier::ElementalType elemental_type = ElementalTypeFromString(elemental_type_str);

	return StatModifier(mod_value, mod_type, elemental_type);
}

EquipmentLoader::EquipmentLoader(const std::string& filepath, bool weapons)
	: m_sFilepath{filepath}
	, m_bWeaponLoader{weapons}
{
	m_pXMLDoc = std::make_unique<tinyxml2::XMLDocument>();
}

EquipmentLoader::~EquipmentLoader()
{
}

std::shared_ptr<Equipment> EquipmentLoader::CreateObjectFromFile(const std::string& objName)
{
	if (LoadFile(m_sFilepath) != XML_SUCCESS)
	{
		std::string error{m_pXMLDoc->ErrorStr()};
		TRPG_ERROR("Failed to load the equipement file - [" + m_sFilepath + "] -- " + error);
		return nullptr;
	}

	// Get the root element
	XMLElement* pRootElement = m_pXMLDoc->RootElement();

	if (!pRootElement)
	{
		std::string error{m_pXMLDoc->ErrorStr()};
		TRPG_ERROR("Failed to get the root element for - [" + m_sFilepath + "] -- " + error);
		return nullptr;
	}

	// Get the equipment
	XMLElement* pEquipment = pRootElement->FirstChildElement(m_bWeaponLoader ? "Weapons" : "Arms");

	if (!pEquipment)
	{
		std::string error{m_pXMLDoc->ErrorStr()};
		TRPG_ERROR("Failed to to get the equipment" + error);
		return nullptr;
	}

	// Get the first Item
	XMLElement* pItem = pEquipment->FirstChildElement(m_bWeaponLoader ? "Weapon" : "Armor");
	if (!pItem)
	{
		std::string error{m_pXMLDoc->ErrorStr()};
		TRPG_ERROR("Failed to get the first item!" + error);
		return nullptr;
	}

	std::shared_ptr<Equipment> newEquipment{nullptr};

	// Loop until we find the item we want to create
	while (pItem)
	{
		XMLElement* pName = pItem->FirstChildElement("Name");
		if (!pName)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get Name of the item!" + error);
			return nullptr;
		}

		// Get the items name
		std::string name{pName->GetText()};
		if (name != objName)
		{
			pItem = pItem->NextSiblingElement(m_bWeaponLoader ? "Weapon" : "Armor");
			continue;
		}

		std::wstring equipName = CharToWide(name.c_str());

		// Get the type
		XMLElement* pType = pItem->FirstChildElement("Type");

		if (!pType)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get type of the item!" + error);
			return nullptr;
		}

		Equipment::EquipType equipType{EquipTypeFromString(pType->GetText())};

		// Get the description
		XMLElement* pDesc = pItem->FirstChildElement("Description");
		if (!pDesc)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get description of the item!" + error);
			return nullptr;
		}
		std::wstring desc = CharToWide(pDesc->GetText());

		// Get the buy price
		XMLElement* pBuyPrice = pItem->FirstChildElement("BuyPrice");
		if (!pBuyPrice)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get buy price of the item!" + error);
			return nullptr;
		}

		int buy_price = atoi(pBuyPrice->GetText());

		if (buy_price < 1)
		{
			TRPG_ERROR("Failed to convert the buy price or price is not in the xml file");
			return nullptr;
		}

		// Get the Equipment properties
		WeaponProperties weapon_props = CreateWeaponProperties(pItem);
		ArmorProperties armor_props = CreateArmorProperties(pItem);
		StatModifier stat_mod = CreateStatModifier(pItem);

		newEquipment =
			ItemCreator::CreateEquipment(equipType, weapon_props, armor_props, stat_mod, equipName, desc, buy_price);

		return newEquipment;
	}

	// If we get here, we did not create an item
	TRPG_ERROR("Item - [" + objName + "] - Does not exist");
	assert(newEquipment);

	return nullptr;
}
