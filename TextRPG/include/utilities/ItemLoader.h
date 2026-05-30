#pragma once
#include "Parser.h"
#include "items/Item.h"

class ItemLoader : public Parser<Item>
{
  public:
	ItemLoader(const std::string& filepath);
	~ItemLoader();

	std::shared_ptr<Item> CreateObjectFromFile(const std::string& objName) override;
	void SaveObjectToFile(Item* obj) override;

private:
	std::string m_sFilepath;
};
