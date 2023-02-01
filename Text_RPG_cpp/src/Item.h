#pragma once
#include <string>

class Player;

class Item
{
public: enum ItemType { HEALTH = 0, STATUS_AFFECT, REVIVE, BATTLE, KEY_ITEM, NO_TYPE };
private:
	const int MAX_COUNT = 99;
	ItemType m_eItemType;
protected:
	int m_Count{ 1 }, m_BuyPrice{0}, m_SellPrice{0}, m_ItemValue{0};
	std::wstring m_sItemName{ L"Item_Name" };
	std::wstring m_sDescription{ L"Item description goes here!" };

	void SetType(ItemType type) { m_eItemType = type; }

public:
	virtual ~Item() {}
	virtual bool OnUse(Player& player) = 0;

	const int GetCount() const { return m_Count; }

	void Decrement()
	{
		if (m_Count > 0) 
			m_Count--;
		if (m_Count <= 0)
			m_Count = 0;
	}
	
	bool AddItem(int num) {
		if (m_Count + num > MAX_COUNT)
			return false;

		m_Count += num;
		return true;
	}

	const std::wstring& GetItemName() const { return m_sItemName; }
	const std::wstring& GetDescription() const { return m_sDescription; }
	const int GetBuyPrice() const { return m_BuyPrice; }
	const int GetSellPrice() const { return m_SellPrice; }
};