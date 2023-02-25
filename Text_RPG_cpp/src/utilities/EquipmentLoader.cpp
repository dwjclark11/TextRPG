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
    const std::string weapon_type_str = pWeaponType ? std::string{ pWeaponType->GetText() } : "";
    WeaponProperties::WeaponType type = WeaponTypeFromString(weapon_type_str);

    return WeaponProperties(attackPwr, type);
}

ArmorProperties EquipmentLoader::CreateArmorProperties(tinyxml2::XMLElement* xmlElement)
{
    return ArmorProperties();
}

StatModifier EquipmentLoader::CreateStatModifier(tinyxml2::XMLElement* xmlElement)
{
    return StatModifier();
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
    return std::shared_ptr<Equipment>();
}
