#pragma once
#include "../Item.h"
#include "Parser.h"

class ItemLoader : public Parser<Item>
{
  private:
	std::string m_sFilepath;

  public:
	ItemLoader(const std::string& filepath);
	~ItemLoader();

	std::shared_ptr<Item> CreateObjectFromFile(const std::string& objName) override;
};