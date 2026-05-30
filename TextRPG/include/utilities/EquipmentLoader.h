#pragma once
#include "items/Equipment.h"
#include "Parser.h"

class EquipmentLoader : public Parser<Equipment>
{

public:
	EquipmentLoader(const std::string& filepath, bool weapons = true);
	~EquipmentLoader();

	std::shared_ptr<Equipment> CreateObjectFromFile(const std::string& objName) override;
	void SaveObjectToFile(Equipment* obj) override {/*NOTHING TO DO HERE*/ };

private:
	WeaponProperties CreateWeaponProperties(tinyxml2::XMLElement* xmlElement);
	ArmorProperties CreateArmorProperties(tinyxml2::XMLElement* xmlElement);
	StatModifier CreateStatModifier(tinyxml2::XMLElement* xmlElement);

private:
	std::string m_sFilepath;
	bool m_bWeaponLoader;
};
