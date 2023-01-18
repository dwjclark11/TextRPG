#pragma once
#include <string>

class Player;

class Item
{
public: enum ItemType { HEALTH = 0, STATUS_AFFECT, REVIVE, BATTLE, KEY_ITEM, NO_TYPE };
private:
	ItemType m_eItemType;
protected:
	int m_Count{ 1 };
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
	
	void AddItem(int num) { m_Count += num; }

	const std::wstring& GetItemName() const { return m_sItemName; }
	const std::wstring& GetDescription() const { return m_sDescription; }
};