#include "ShopLoader.h"
#include "../Logger.h"
#include "../utilities/trpg_utilities.h"
#include "ItemCreator.h"
#include "EquipmentLoader.h"
#include "ItemLoader.h"

using namespace tinyxml2;

ShopLoader::ShopLoader()
{
	m_pXMLDoc = std::make_unique<tinyxml2::XMLDocument>();
}

ShopLoader::~ShopLoader()
{
}

std::unique_ptr<ShopParameters> ShopLoader::CreateShopParametersFromFile(const std::string& shop_filepath)
{
	if (LoadFile(shop_filepath) != XMLError::XML_SUCCESS)
	{
		TRPG_ERROR("Failed to load shop parameters from [" + shop_filepath + "]");
		return nullptr;
	}

	// Get the root Element
	XMLElement* pRootElement = m_pXMLDoc->RootElement();

	if (!pRootElement)
	{
		TRPG_ERROR("Failed to get the root element");
		return nullptr;
	}

	// Get the shop parameters
	XMLElement* pShopParams = pRootElement->FirstChildElement("ShopParameters");

	if (!pShopParams)
	{
		TRPG_ERROR("Failed to get the shop parameters");
		return nullptr;
	}

	// Get the shop type
	XMLElement* pShopType = pShopParams->FirstChildElement("ShopType");

	if (!pShopType)
	{
		TRPG_ERROR("Failed to get the shop type");
		return nullptr;
	}

	// Shop type string
	std::string shopTypeStr = pShopType->GetText();

	// Get the inventory
	XMLElement* pInventory = pShopParams->FirstChildElement("Inventory");

	if (!pInventory)
	{
		TRPG_ERROR("Failed to get the shop inventory");
		return nullptr;
	}

	std::unique_ptr<Inventory> inventory = std::make_unique<Inventory>();
	ShopParameters::ShopType shopType = ShopTypeFromString(shopTypeStr);
	std::string definitionLocation{""};
	bool itemLoader{false};
	bool weaponLoader{false};

	switch (shopType)
	{
	case ShopParameters::ShopType::WEAPON:
		definitionLocation = "./assets/xml_files/WeaponDefs.xml";
		weaponLoader = true;
		break;
	case ShopParameters::ShopType::ARMOR: definitionLocation = "./assets/xml_files/ArmorDefs.xml"; break;
	case ShopParameters::ShopType::ACCESSORY: definitionLocation = "./assets/xml_files/AccessoryDefs.xml"; break;
	case ShopParameters::ShopType::ITEM:
		definitionLocation = "./assets/xml_files/ItemDefs.xml";
		itemLoader = true;
		break;
	case ShopParameters::ShopType::NOT_A_SHOP: TRPG_ERROR("Shop Type is not a known type!") return nullptr;
	}

	// Create the item loader if item shop
	std::unique_ptr<ItemLoader> itemLoaderPtr{nullptr};
	if (itemLoader)
		itemLoaderPtr = std::make_unique<ItemLoader>(definitionLocation);

	// Create the equipment loader if equipment shop
	std::unique_ptr<EquipmentLoader> equipmentLoader{nullptr};
	if (!itemLoader)
		equipmentLoader = std::make_unique<EquipmentLoader>(definitionLocation, weaponLoader);

	// Get the shop items
	XMLElement* pShopItem = pInventory->FirstChildElement("ShopItem");
	if (!pShopItem)
	{
		TRPG_ERROR("Failed to get the Shop items");
		return nullptr;
	}

	// Get the first item
	XMLElement* pItem = pShopItem->FirstChildElement(shopTypeStr.c_str());
	if (!pItem)
	{
		TRPG_ERROR("Failed to get the first item in the inventory");
		return nullptr;
	}

	while (pItem)
	{
		// Get the item name
		XMLElement* pName = pItem->FirstChildElement("Name");
		if (!pName)
		{
			TRPG_ERROR("Failed to get the item name for shop items!");
			return nullptr;
		}

		std::string name{pName->GetText()};

		if (itemLoader)
		{
			auto newItem = itemLoaderPtr->CreateObjectFromFile(name);

			if (newItem)
				inventory->AddItem(std::move(newItem));
		}
		else // Load Equipment
		{
			auto newEquipment = equipmentLoader->CreateObjectFromFile(name);
			if (newEquipment)
				inventory->AddEquipment(std::move(newEquipment));
		}

		// Move to the next shop Item
		pItem = pItem->NextSiblingElement(shopTypeStr.c_str());
	}

	// Create the shop parameters
	std::unique_ptr<ShopParameters> shop_parameters = std::make_unique<ShopParameters>(std::move(inventory), shopType);

	if (!shop_parameters)
	{
		TRPG_ERROR("Failed to create the shop Parameters!");
		return nullptr;
	}

	return shop_parameters;
}

std::shared_ptr<ShopParameters> ShopLoader::CreateObjectFromFile(const std::string& objName)
{
	return std::shared_ptr<ShopParameters>();
}
