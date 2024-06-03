#include "ItemLoader.h"
#include "trpg_utilities.h"
#include "ItemCreator.h"
#include "../Potion.h"
#include "../Logger.h"

#include <cassert>

using namespace tinyxml2;

ItemLoader::ItemLoader(const std::string& filepath)
	: m_sFilepath{filepath}
{
	m_pXMLDoc = std::make_unique<tinyxml2::XMLDocument>();
}

ItemLoader::~ItemLoader()
{
}

std::shared_ptr<Item> ItemLoader::CreateObjectFromFile(const std::string& objName)
{
	if (LoadFile(m_sFilepath) != XML_SUCCESS)
	{
		std::string error{m_pXMLDoc->ErrorStr()};
		TRPG_ERROR("Failed to load Item file -[" + m_sFilepath + "] -- " + error);
		return nullptr;
	}

	// Get Root element
	XMLElement* pRootElement = m_pXMLDoc->RootElement();

	if (!pRootElement)
	{
		std::string error{m_pXMLDoc->ErrorStr()};
		TRPG_ERROR("Failed to get the root element! - " + error);
		return nullptr;
	}

	// Get the Items
	XMLElement* pItems = pRootElement->FirstChildElement("Items");

	if (!pItems)
	{
		std::string error{m_pXMLDoc->ErrorStr()};
		TRPG_ERROR("Failed to get the Items! - " + error);
		return nullptr;
	}

	// Get the first Item
	XMLElement* pItem = pItems->FirstChildElement("Item");

	if (!pItem)
	{
		std::string error{m_pXMLDoc->ErrorStr()};
		TRPG_ERROR("Failed to get the first Item! - " + error);
		return nullptr;
	}

	std::shared_ptr<Item> newItem{nullptr};

	while (pItem)
	{
		XMLElement* pName = pItem->FirstChildElement("Name");
		if (!pName)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get the item name! - " + error);
			return nullptr;
		}

		// Get the item name
		std::string name{pName->GetText()};

		if (name != objName)
		{
			pItem = pItem->NextSiblingElement("Item");
			continue;
		}

		// Get the item type
		XMLElement* pType = pItem->FirstChildElement("Type");
		if (!pType)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get the item type! - " + error);
			return nullptr;
		}

		Item::ItemType type = ItemTypeFromString(pType->GetText());

		// Get the item description
		XMLElement* pDesc = pItem->FirstChildElement("Description");
		if (!pDesc)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get the item description! - " + error);
			return nullptr;
		}

		std::wstring desc = CharToWide(pDesc->GetText());

		// Get the item value
		XMLElement* pValue = pItem->FirstChildElement("Value");
		if (!pValue)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get the item value! - " + error);
			return nullptr;
		}

		int value = atoi(pValue->GetText());

		if (value < -1)
		{
			TRPG_ERROR("Item value below minimum!");
			return nullptr;
		}

		// Get the item buy price
		XMLElement* pBuyPrice = pItem->FirstChildElement("BuyPrice");
		if (!pBuyPrice)
		{
			std::string error{m_pXMLDoc->ErrorStr()};
			TRPG_ERROR("Failed to get the item buy price! - " + error);
			return nullptr;
		}

		int buy_price = atoi(pBuyPrice->GetText());
		if (buy_price < 1)
		{
			TRPG_ERROR("Item buy price was below one!");
			return nullptr;
		}

		newItem = ItemCreator::CreateItem(type, CharToWide(name.c_str()), desc, value, buy_price);
		return newItem;
	}

	// If we get here ObjName does not exist!
	assert(newItem);

	return nullptr;
}
