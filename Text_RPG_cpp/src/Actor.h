#pragma once
#include <string>
#include <vector>

class Actor
{
public:
	enum class ActorType
	{
		WARRIOR = 0, BLACK_MAGE, THIEF,
	};
private:
	const std::vector<std::wstring> m_EquipmentSlotLabels {L"Weapon:", L"Head Gear:", L"Armor:", L"Foot Wear:", L"Accessory:"};
	// TODO: std::vector<std::wstring> m_StatLabels;

protected:
	std::wstring m_sName;
	int m_Level, m_XP, m_XPToNextLevel, m_HP, m_MaxHP;

	bool m_bDead;
	ActorType m_eActorType;

	// TODO: 
	/*
		- map to equipment type of equiped items -- We need our equipment
		- Stats == Create a class
		- Actions
	*/

public:
	Actor();
	Actor(const std::wstring& name, int level, int max_hp, ActorType type = ActorType::WARRIOR);

	~Actor() = default;

	inline const int GetHP() const { return m_HP; }
	inline const int GetMaxHP() const { return m_MaxHP; }

	inline const std::vector<std::wstring>& GetEquipSlotLabels() const { return m_EquipmentSlotLabels; }
	inline const bool IsDead() const { return m_bDead; }
	inline const std::wstring& GetName() const { return m_sName; }
	const int GetLevel() const { return m_Level; }
	const int GetXP() const { return m_XP; }
	const int GetXPToNextLevel() const { return m_XPToNextLevel; }


	void HealHP(int hp);
	void TakeDamage(int damage);


};