#include "Actor.h"

Actor::Actor()
	: Actor(L"Actor_name", L"Actor_ID", 1, 100, ActorType::WARRIOR)
{
}

Actor::Actor(const std::wstring& name, const std::wstring& id, int level, int max_hp, ActorType type)
	: m_sName{ name }
	, m_sID{id}
	, m_Level{level}
	, m_XP{0}
	, m_XPToNextLevel{ 250 }
	, m_HP{max_hp}
	, m_MaxHP{max_hp}, m_MP{5}, m_MaxMP{5}
	, m_bDead{false}
	, m_eActorType{type}
	, m_mapEquipmentSlots{
		{ Stats::EquipSlots::WEAPON, nullptr},
		{ Stats::EquipSlots::HEADGEAR, nullptr},
		{ Stats::EquipSlots::CHEST_BODY, nullptr},
		{ Stats::EquipSlots::FOOTWEAR, nullptr},
		{ Stats::EquipSlots::ACCESSORY, nullptr},
	}
	, m_Stats{10, 3, 5, 5, 7} // TEST STATS

{
}

void Actor::HealHP(int hp)
{
	m_HP += hp;

	if (m_HP >= m_MaxHP)
		m_HP = m_MaxHP;
}

void Actor::TakeDamage(int damage)
{
	m_HP -= damage;
	if (m_HP <= 0)
	{
		m_HP = 0;
		m_bDead = true;
	}
}

bool Actor::UseMP(int mp)
{
	if (mp > m_MP)
		return false;

	m_MP -= mp;

	if (m_MP <= 0)
		m_MP = 0;

	return true;
}
