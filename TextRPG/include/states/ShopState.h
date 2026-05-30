#pragma once
#include "IState.h"
#include "ui/Selector.h"

struct ShopParameters;
class Item;
class Equipment;

class ShopState : public IState
{
  public:
	ShopState(StateContext& context, const std::string& shopFilepath);
	~ShopState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;

private:
	void DrawShop();
	void DrawBuyItems();
	void DrawItemsBox();
	void ResetSelections();

	void BuyEquipment();
	void SellEquipment();
	void BuyItems();
	void SellItems();

	// Selector Functions
	void OnShopMenuSelect(int index, std::vector<std::wstring> data);
	void BuySellOptions(int index, std::vector<std::wstring> data);

	void OnBuyItemSelect(int index, std::vector<std::shared_ptr<Item>> data);
	void OnBuyEquipmentSelect(int index, std::vector<std::shared_ptr<Equipment>> data);

	void OnSellItemSelect(int index, std::vector<std::shared_ptr<Item>> data);
	void OnSellEquipmentSelect(int index, std::vector<std::shared_ptr<Equipment>> data);

	void RenderBuyItems(int x, int y, std::shared_ptr<Item> item);
	void RenderBuyEquipment(int x, int y, std::shared_ptr<Equipment> item);

	void RenderSellItems(int x, int y, std::shared_ptr<Item> item);
	void RenderSellEquipment(int x, int y, std::shared_ptr<Equipment> item);

	void UpdateBuyQuantity(int price);
	void UpdateSellQuantity(int totalAvailable);

private:
	StateContext& m_Context;

	std::unique_ptr<ShopParameters> m_pShopParameters;
	Selector<> m_ShopChoiceSelector;
	Selector<> m_BuySellSelector;
	Selector<std::shared_ptr<Equipment>> m_EquipmentSelector;
	Selector<std::shared_ptr<Item>> m_ItemSelector;

	int m_Quantity;
	int m_Price;
	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_CenterScreenW;
	int m_PanelBarX;
	int m_AvailableSellQunatity;

	bool m_bInShopSelect;
	bool m_bInItemBuy;
	bool m_bInItemSell;
	bool m_bSetFuncs;
	bool m_bIsEquipmentShop;
	bool m_bExitShop;
	bool m_bBuySellItem;
};
