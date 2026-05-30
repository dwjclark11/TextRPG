#pragma once
#include "Parser.h"
#include "ShopParameters.h"

class ShopLoader : public Parser<ShopParameters>
{
  public:
	ShopLoader();
	~ShopLoader();

	std::unique_ptr<ShopParameters> CreateShopParametersFromFile(const std::string& shop_filepath);
	
	std::shared_ptr<ShopParameters> CreateObjectFromFile(const std::string& objName) override;
	void SaveObjectToFile(ShopParameters*) override {/*NOTHING TO DO HERE*/ };
};
