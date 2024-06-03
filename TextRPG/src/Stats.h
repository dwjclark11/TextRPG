#pragma once
#include <unordered_map>
#include <string>

class Stats
{
  public:
	enum class EquipSlots
	{
		WEAPON = 0,
		HEADGEAR,
		CHEST_BODY,
		FOOTWEAR,
		ACCESSORY,
		NO_SLOT
	};

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

	/*
	 * GetStat(key)
	 * @param - Takes in a std::wstring as the key
	 * @return - This returns the stat value as an integer based on the given key value; If the key does not exist,
	 * it displays a warning in the log and returns 0.
	 */
	const int GetStat(const std::wstring& key);

	/*
	 * GetModifier(key)
	 * @param - Takes in a std::wstring as the key
	 * @return - This returns the modifier value as an integer based on the given key value; If the key does not exist,
	 * it displays a warning in the log and returns 0.
	 */
	const int GetModifier(const std::wstring& key);

	/*
	 * GetEquipmentValue(slot)
	 * @param - Takes in a EquipSlots enum.
	 * @return - This returns the equipment value as an integer based on the given slot value; If the slot does not
	 * exist, it displays a warning in the log and returns 0.
	 */
	const int GetEquipmentValue(EquipSlots slot);

	/*
	 * SetModifier(key, value)
	 * @param - Takes in a const std::wstring as the key to the stat modifier map
	 * @param - Takes in an integer value as the new value for that stat modifier.
	 * If the Stat modifier does not exist in the modifier map, this will log an error and return.
	 */
	void SetModifier(const std::wstring& key, int value);

	/*
	 * SetEquipmentValue(slot, value)
	 * @param - Takes in a EquipSlots enum as the key to the equipment map
	 * @param - Takes in an integer value as the new value for that slot.
	 * If the slot does not exist in the equipment map, this will log an error and return.
	 */
	void SetEquipmentValue(EquipSlots slot, int value);

	/*
	 * SetStat(key, value)
	 * @param - Takes in a const std::wstring as the key to the stat map
	 * @param - Takes in an integer value as the new value for that stat.
	 * If the Stat does not exist in the map, this will log an error and return.
	 */
	void SetStat(const std::wstring& key, int value);

	/*
	 * UpdateStats();
	 * This is to be used whenever there is a change to one of the stats, stat modifiers, or equipped weapons.
	 * The Attack and Defense stats are calculated based on the current player stats, modifiers and equipment.
	 */
	void UpdateStats();
};