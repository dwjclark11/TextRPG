#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Stats.h"
#include "Equipment.h"

class Actor
{
  public:
	enum class ActorType
	{
		WARRIOR = 0,
		BLACK_MAGE,
		THIEF,
		FIGHTER,
	};

  private:
	const std::vector<std::wstring> m_EquipmentSlotLabels{L"Weapon", L"Headgear", L"Armor", L"Footwear", L"Accessory"};
	const std::vector<std::wstring> m_StatLabels{
		L"Attack", L"Strength", L"Intelligence", L"Speed", L"Dexterity", L"Stamina"};

  protected:
	std::wstring m_sName, m_sID;
	int m_Level, m_XP, m_XPToNextLevel, m_HP, m_MaxHP;
	int m_MP, m_MaxMP;

	bool m_bDead;
	ActorType m_eActorType;

	std::unordered_map<Stats::EquipSlots, std::shared_ptr<Equipment>> m_mapEquipmentSlots;

	Stats m_Stats;

  public:
	Actor();
	Actor(const std::wstring& name, const std::wstring& id, int level, int max_hp, ActorType type = ActorType::WARRIOR);

	~Actor() = default;

	inline const int GetHP() const { return m_HP; }
	inline const int GetMaxHP() const { return m_MaxHP; }

	/*
	 * @return This returns a const std::vector<std::wstring>& for drawing the Equipment slot labels for
	 * each actor. The slots will never need to be changed.
	 */
	inline const std::vector<std::wstring>& GetEquipSlotLabels() const { return m_EquipmentSlotLabels; }
	inline const std::vector<std::wstring>& GetStatLabels() const { return m_StatLabels; }

	std::unordered_map<Stats::EquipSlots, std::shared_ptr<Equipment>>& GetEquippedItemsSlots()
	{
		return m_mapEquipmentSlots;
	}

	Stats& GetStats() { return m_Stats; }

	inline const bool IsDead() const { return m_bDead; }
	inline const std::wstring& GetName() const { return m_sName; }
	inline const std::wstring& GetID() const { return m_sID; }

	const int GetLevel() const { return m_Level; }
	const int GetXP() const { return m_XP; }
	const int GetMP() const { return m_MP; }
	const int GetMaxMP() const { return m_MaxMP; }

	/*
	 * @return This returns the amount of XP required for leveling up as an integer value.
	 */
	const int GetXPToNextLevel() const { return m_XPToNextLevel; }

	/*
	 * Used to heal the Actors HP.
	 * @param - Takes in the amount of HP as an integer to heal and checks to see if the current HP
	 * is >= MaxHP. If it is, sets it to MaxHP.
	 */
	void HealHP(int hp);

	/*
	 * Used to take damage from the Actor's HP
	 * @param - Takes in the amount of damage to remove from the Actor's HP as an integer.
	 * It also checks to see if the actor's HP is below zero. If so, it sets the actor's HP
	 * to zero and sets the dead flag.
	 */
	void TakeDamage(int damage);

	bool UseMP(int mp);
};