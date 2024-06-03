#pragma once
#include "../Equipment.h"
#include "Parser.h"

class EquipmentLoader : public Parser<Equipment>
{
  private:
	std::string m_sFilepath;
	bool m_bWeaponLoader;

	WeaponProperties CreateWeaponProperties(tinyxml2::XMLElement* xmlElement);
	ArmorProperties CreateArmorProperties(tinyxml2::XMLElement* xmlElement);
	StatModifier CreateStatModifier(tinyxml2::XMLElement* xmlElement);

  public:
	EquipmentLoader(const std::string& filepath, bool weapons = true);
	~EquipmentLoader();

	std::shared_ptr<Equipment> CreateObjectFromFile(const std::string& objName) override;
};