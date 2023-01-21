#pragma once
#include <unordered_map>
#include <string>

class Stats
{
public: enum class EquipSlots { WEAPON = 0, HEADGEAR, CHEST_BODY, FOOTWEAR, ACCESSORY, NO_SLOT };

private:
	std::unordered_map<std::wstring, int> m_StatList;
	std::unordered_map<std::wstring, int> m_StatModifierList;
	std::unordered_map<EquipSlots, int> m_EquipSlotList;

public: 
	Stats();
	Stats(int strength, int intelligence, int speed, int dexterity, int stamina);
	~Stats() = default;

	inline const std::unordered_map<std::wstring, int>& GetStatList() const { return m_StatList; }
	inline const std::unordered_map<std::wstring, int>& GetModifierList() const { return m_StatModifierList; }
	inline const std::unordered_map<EquipSlots, int>& GetEquipSlotList() const { return m_EquipSlotList; }

	const int GetStat(const std::wstring& key);
	const int GetModifier(const std::wstring& key);
	const int GetEquipmentValue(EquipSlots slot);

	void SetModifier(const std::wstring& key, int value);
	void SetEquipmentValue(EquipSlots slot, int value);
	void SetStat(const std::wstring& key, int value);

	void UpdateStats();
};