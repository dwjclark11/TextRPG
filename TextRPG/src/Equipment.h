#pragma once
#include <string>

class Player;

struct StatModifier
{
	enum class ModifierType
	{
		STRENGTH = 0,
		SPEED,
		INTELLIGENCE,
		DEXTERITY,
		STAMINA,
		ELEMENTAL,
		NO_TYPE
	};
	enum class ElementalType
	{
		FIRE = 0,
		EARTH,
		WIND,
		ICE,
		WATER,
		LIGHTNING,
		NO_TYPE
	};

	int statModifierVal;
	ModifierType modifierType;
	ElementalType elementalType;
	std::wstring modifierTypeStr;

	StatModifier(int val = 0, ModifierType mod_type = ModifierType::NO_TYPE,
				 ElementalType elemental_type = ElementalType::NO_TYPE)
		: statModifierVal{val}
		, modifierType{mod_type}
		, elementalType{elemental_type}
	{
		switch (mod_type)
		{
		case StatModifier::ModifierType::STRENGTH: modifierTypeStr = L"Strength"; break;
		case StatModifier::ModifierType::SPEED: modifierTypeStr = L"Speed"; break;
		case StatModifier::ModifierType::INTELLIGENCE: modifierTypeStr = L"Intelligence"; break;
		case StatModifier::ModifierType::DEXTERITY: modifierTypeStr = L"Dexterity"; break;
		case StatModifier::ModifierType::STAMINA: modifierTypeStr = L"Stamina"; break;
		case StatModifier::ModifierType::ELEMENTAL: modifierTypeStr = L"Elemental"; break;
		case StatModifier::ModifierType::NO_TYPE: modifierTypeStr = L"No_Type"; break;
		default: break;
		}
	}
};

struct WeaponProperties
{
	enum class WeaponType
	{
		SWORD = 0,
		DAGGER,
		BOW,
		STAFF,
		NOT_A_WEAPON
	};
	int attackPwr;
	WeaponType weaponType;

	WeaponProperties(int attack_pwr = 0, WeaponType weapon_type = WeaponType::NOT_A_WEAPON)
		: attackPwr{attack_pwr}
		, weaponType{weapon_type}
	{
	}
};

struct ArmorProperties
{
	enum class ArmorType
	{
		HEADGEAR = 0,
		CHEST_BODY,
		FOOTWEAR,
		NOT_ARMOR
	};
	int defensePwr;
	ArmorType armorType;

	ArmorProperties(int defense_pwr = 0, ArmorType armor_type = ArmorType::NOT_ARMOR)
		: defensePwr{defense_pwr}
		, armorType{armor_type}
	{
	}
};

class Equipment
{
  public:
	enum class EquipType
	{
		WEAPON = 0,
		ARMOR,
		ACCESSORY,
		NO_TYPE
	};

  private:
	const int MAX_COUNT = 50;

	Equipment::EquipType m_eEquipType{EquipType::NO_TYPE};

  protected:
	std::wstring m_sName{L""}, m_sDescription{L""};
	int m_BuyPrice{0}, m_SellPrice{0}, m_Count{1}, m_Weight{0};
	bool m_bEquipped{false};
	WeaponProperties m_WeaponProperties;
	ArmorProperties m_ArmorProperties;
	StatModifier m_StatModifier;

	void SetEquipType(EquipType type) { m_eEquipType = type; }

  public:
	virtual ~Equipment() {}

	/*
	 * @brief Gets the value of the equipment. Atk Pwr if weapon,
	 * Def Pwr if Armor, etc.
	 * @return Returns a const int of the value.
	 */
	virtual const int GetValue() const = 0;

	/*
	 * @brief This function needs to be overridden for all equipment.
	 * Adds all stat modifiers and equipment values to the player stats.
	 * @param Takes in the Player& that it is equipped to.
	 * @return returns true if successful, false otherwise.
	 */
	virtual bool OnEquip(Player& player) = 0;

	/*
	 * This function needs to be overridden for all equipment.
	 * Removes all stat modifiers and equipment values from the player.
	 * @param Takes in the Player& that it is equipped to.
	 * @return returns true if successful, false otherwise.
	 */
	virtual bool OnRemove(Player& player) = 0;

	/*
	 * @brief Sets equipped to false;
	 */
	inline void Remove()
	{
		if (m_bEquipped)
			m_bEquipped = false;
	}

	/*
	 * @brief Sets equipped to true;
	 */
	inline void Equip()
	{
		if (!m_bEquipped)
			m_bEquipped = true;
	}

	/*
	 * @brief Checks to see if the Equipment is currently equipped.
	 * @return Returns true if the item is equipped, false otherwise.
	 */
	inline const bool IsEquipped() const { return m_bEquipped; }

	inline const std::wstring& GetName() const { return m_sName; }
	inline const std::wstring& GetDescription() const { return m_sDescription; }

	inline bool Add(int num = 1)
	{
		if (m_Count + num > MAX_COUNT)
			return false;

		m_Count += num;
		return true;
	}

	inline bool Decrement(int num = 1)
	{
		if (m_Count <= 0)
			return false;
		if (m_Count - num < 0)
			return false;

		m_Count--;

		if (m_Count < 0)
			m_Count = 0;

		return true;
	}

	inline const int GetCount() const { return m_Count; }
	inline const Equipment::EquipType GetType() const { return m_eEquipType; }
	inline const WeaponProperties& GetWeaponProperties() const { return m_WeaponProperties; }
	inline const ArmorProperties& GetArmorProperties() const { return m_ArmorProperties; }
	inline const StatModifier& GetStatModifier() const { return m_StatModifier; }
	inline const int GetBuyPrice() const { return m_BuyPrice; }
	inline const int GetSellPrice() const { return m_SellPrice; }
	inline const int GetMaxCount() const { return MAX_COUNT; }
};

class Weapon : public Equipment
{
  public:
	Weapon();
	Weapon(const std::wstring& name, const std::wstring& description, int buy_price,
		   WeaponProperties weapon_properties = WeaponProperties(), StatModifier stat_modifier = StatModifier());
	~Weapon() = default;

	inline const int GetValue() const override { return m_WeaponProperties.attackPwr; }
	bool OnEquip(Player& player) override;
	bool OnRemove(Player& player) override;
};

class Armor : public Equipment
{
  public:
	Armor();
	Armor(const std::wstring& name, const std::wstring& description, int buy_price,
		  ArmorProperties armor_properties = ArmorProperties(), StatModifier stat_modifier = StatModifier());
	~Armor() = default;

	inline const int GetValue() const override { return m_ArmorProperties.defensePwr; }
	bool OnEquip(Player& player) override;
	bool OnRemove(Player& player) override;
};
