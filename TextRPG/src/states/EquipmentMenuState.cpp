#include "EquipmentMenuState.h"
#include "../Player.h"
#include "../Console.h"
#include "StateMachine.h"
#include "../inputs/Keyboard.h"
#include <cassert>

using namespace std::placeholders;

void EquipmentMenuState::DrawEquipment()
{
	// Draw Opening Bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 1, PANEL_BARS + 2, YELLOW);
	int menu_x_pos = m_CenterScreenW - (EQUIP_SIZE / 2);
	m_Console.Write(menu_x_pos, 2, L" _____            _                            _  ", GREEN);
	m_Console.Write(menu_x_pos, 3, L"| ____|__ _ _   _(_)_ __  _ __ ___   ___ _ __ | |_ ", GREEN);
	m_Console.Write(menu_x_pos, 4, L"|  _| / _` | | | | | '_ \\| '_ ` _ \\ / _ \\ '_ \\| __|", GREEN);
	m_Console.Write(menu_x_pos, 5, L"| |__| (_| | |_| | | |_) | | | | | |  __/ | | | |_", GREEN);
	m_Console.Write(menu_x_pos, 6, L"|_____\\__, |\\__,_|_| .__/|_| |_| |_|\\___|_| |_|\\__|", GREEN);
	m_Console.Write(menu_x_pos, 7, L"         |_|       |_|                             ", GREEN);

	m_Console.DrawPanelHorz(m_PanelBarX - 1, 9, PANEL_BARS + 2, YELLOW);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 11, PANEL_BARS + 2, YELLOW);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 13, PANEL_BARS + 2, YELLOW);

	// Player holder
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 15), PANEL_BARS + 2, YELLOW);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 17), PANEL_BARS + 2, YELLOW);
	// Draw Closing Bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 2), PANEL_BARS + 2, YELLOW);

	// Draw Right bar
	m_Console.DrawPanelVert(m_PanelBarX - 1, 2, 44, YELLOW);
	m_Console.DrawPanelVert(m_PanelBarX + PANEL_BARS, 2, 44, YELLOW);

	m_Console.Write(30, 21, L"ATTRIBUTES", YELLOW);
	m_Console.Write(30, 22, L"==========", YELLOW);
}

void EquipmentMenuState::DrawPlayerInfo()
{
	const auto& name = m_Player.GetName();
	m_Console.Write(m_CenterScreenW - static_cast<int>(name.size() / 2), 12, name);

	const auto& stats_list = m_Player.GetStats().GetStatList();
	int i = 0;
	for (const auto& [ stat, value ] : stats_list)
	{
		const auto& mod_value = m_Player.GetStats().GetModifier(stat);
		m_Console.Write(STAT_LABEL_X_POS, STAT_LABEL_START_Y_POS + i, stat);
		m_Console.Write(STAT_VAL_X_POS, STAT_LABEL_START_Y_POS + i, std::to_wstring(value + mod_value));
		DrawStatModifier(STAT_PREDICT_X_POS, STAT_LABEL_START_Y_POS + i, stat, value);
		i++;
	}
}

void EquipmentMenuState::DrawStatPrediction()
{
	if (m_bInSlotSelect)
		return;

	const auto& data = m_EquipmentSelector.GetData();

	if (data.empty())
		return;

	const auto& index = m_EquipmentSelector.GetIndex();
	if (index > data.size() - 1)
	{
		m_Console.Write(75, m_DiffPosY, L"    ");
		return;
	}

	const auto& item = data[ index ];

	if (item->IsEquipped())
		return;

	const auto& current_equipped = m_Player.GetEquippedItemsSlots()[ m_eEquipSlots ];

	int current_equipped_val = 0;

	if (current_equipped)
		current_equipped_val = current_equipped->GetValue();

	const auto& new_item_val = item->GetValue();
	const auto& name = item->GetName();

	int difference = new_item_val - current_equipped_val;
	WORD diff_color = WHITE;
	std::wstring diff_dir = L"=";

	if (difference > 0)
	{
		diff_color = GREEN;
		diff_dir = L"+";
	}
	else if (difference < 0)
	{
		diff_color = RED;
		diff_dir = L"-";
	}

	int abs_diff_val = abs(difference);

	m_Console.Write(STAT_PREDICT_X_POS, m_DiffPosY, diff_dir + L" " + std::to_wstring(abs_diff_val), diff_color);
}

void EquipmentMenuState::DrawStatModifier(int x, int y, const std::wstring& stat, int value)
{
	if (m_bInSlotSelect || m_bInMenuSelect)
		return;

	const auto& data = m_EquipmentSelector.GetData();

	if (data.empty())
		return;

	const auto& index = m_EquipmentSelector.GetIndex();
	if (index > data.size() - 1)
	{
		m_Console.Write(x, m_PrevStatModPos, L"    ");
		return;
	}

	const auto& item = data[ index ];

	if (item->IsEquipped())
		return;

	const auto& stat_modifier = item->GetStatModifier();
	if (stat_modifier.modifierType == StatModifier::ModifierType::NO_TYPE)
		return;

	if (stat != stat_modifier.modifierTypeStr)
	{
		m_Console.Write(x, y, L"    ");
		return;
	}

	const std::wstring stat_val = std::to_wstring(stat_modifier.statModifierVal);
	m_Console.Write(x, y, L" + " + stat_val, GREEN);
	m_PrevStatModPos = y;
}

void EquipmentMenuState::OnMenuSelect(int index, std::vector<std::wstring> data)
{
	if (index > data.size() - 1 || data.empty())
		return;

	switch (index)
	{
	case 0:
		// NOTHING TO DO
		break;
	case 1: m_bRemoveEquipment = true; break;
	case 2:
		// TODO:
		break;
	default: return;
	}
	m_bInMenuSelect = false;
	m_bInSlotSelect = true;
	m_MenuSelector.HideCursor();
	m_EquipSlotSelector.ShowCursor();
}

void EquipmentMenuState::OnEquipSelect(int index, std::vector<std::shared_ptr<Equipment>> data)
{
	if (index > data.size() - 1 || data.empty())
		return;

	// If there is an item equipped in that slot, remove it
	const auto& equippedItem = m_Player.GetEquippedItemsSlots()[ m_eEquipSlots ];
	if (equippedItem)
		equippedItem->Remove();

	const auto& item = data[ index ];

	m_Player.GetEquippedItemsSlots()[ m_eEquipSlots ] = nullptr;
	m_Player.GetEquippedItemsSlots()[ m_eEquipSlots ] = item;

	Equipment::EquipType equipType = Equipment::EquipType::NO_TYPE;
	ArmorProperties::ArmorType armor_type = ArmorProperties::ArmorType::NOT_ARMOR;

	switch (m_eEquipSlots)
	{
	case Stats::EquipSlots::WEAPON: equipType = Equipment::EquipType::WEAPON; break;
	case Stats::EquipSlots::HEADGEAR:
		equipType = Equipment::EquipType::ARMOR;
		armor_type = ArmorProperties::ArmorType::HEADGEAR;
		break;
	case Stats::EquipSlots::CHEST_BODY:
		equipType = Equipment::EquipType::ARMOR;
		armor_type = ArmorProperties::ArmorType::CHEST_BODY;
		break;
	case Stats::EquipSlots::FOOTWEAR:
		equipType = Equipment::EquipType::ARMOR;
		armor_type = ArmorProperties::ArmorType::FOOTWEAR;
		break;
	case Stats::EquipSlots::ACCESSORY: equipType = Equipment::EquipType::ACCESSORY; break;
	default: break;
	}
	if (!item->OnEquip(m_Player))
	{
		TRPG_ERROR("Failed to equip the item on the player");
		return;
	}

	// Reset the Selected Equipment
	std::vector<std::shared_ptr<Equipment>> selectedEquipment{};

	for (const auto& item : m_Player.GetInventory().GetEquipment())
	{
		const auto& type = item->GetType();
		if (type != equipType || item->IsEquipped())
			continue;

		if (type == Equipment::EquipType::ARMOR && item->GetArmorProperties().armorType != armor_type)
			continue;

		selectedEquipment.push_back(item);
	}

	m_EquipmentSelector.SetData(selectedEquipment);
	m_Console.ClearBuffer();
}

void EquipmentMenuState::OnSlotSelect(int index, std::vector<std::wstring> data)
{
	const auto& slot_name = data[ index ];

	Equipment::EquipType equipType = Equipment::EquipType::NO_TYPE;
	ArmorProperties::ArmorType armor_type = ArmorProperties::ArmorType::NOT_ARMOR;
	int statPos = 0;
	if (slot_name == L"Weapon")
	{
		m_eEquipSlots = Stats::EquipSlots::WEAPON;
		equipType = Equipment::EquipType::WEAPON;
	}
	else if (slot_name == L"Headgear")
	{
		m_eEquipSlots = Stats::EquipSlots::HEADGEAR;
		equipType = Equipment::EquipType::ARMOR;
		armor_type = ArmorProperties::ArmorType::HEADGEAR;
		statPos = 1;
	}
	else if (slot_name == L"Armor")
	{
		m_eEquipSlots = Stats::EquipSlots::CHEST_BODY;
		equipType = Equipment::EquipType::ARMOR;
		armor_type = ArmorProperties::ArmorType::CHEST_BODY;
		statPos = 1;
	}
	else if (slot_name == L"Footwear")
	{
		m_eEquipSlots = Stats::EquipSlots::FOOTWEAR;
		equipType = Equipment::EquipType::ARMOR;
		armor_type = ArmorProperties::ArmorType::FOOTWEAR;
		statPos = 1;
	}
	else if (slot_name == L"Accessory")
	{
		m_eEquipSlots = Stats::EquipSlots::ACCESSORY;
		equipType = Equipment::EquipType::ARMOR;
	}
	else
	{
		assert(false, &"SHOULD BE A VALID EQUIP TYPE");
		return;
	}

	if (m_bRemoveEquipment)
	{
		RemoveEquipment(index, data);
		return;
	}

	std::vector<std::shared_ptr<Equipment>> selectedEquipment{};

	for (const auto& item : m_Player.GetInventory().GetEquipment())
	{
		const auto& type = item->GetType();
		if (type != equipType || item->IsEquipped())
			continue;

		if (type == Equipment::EquipType::ARMOR && item->GetArmorProperties().armorType != armor_type)
			continue;

		selectedEquipment.push_back(item);
	}

	m_EquipmentSelector.SetData(selectedEquipment);

	m_DiffPosY = STAT_LABEL_START_Y_POS + statPos;
	m_sCurrentSlot = slot_name;
	m_EquipSlotSelector.HideCursor();
	m_EquipmentSelector.ShowCursor();
	m_bInSlotSelect = false;
}

void EquipmentMenuState::RenderEquip(int x, int y, std::shared_ptr<Equipment> item)
{
	if (item->IsEquipped())
		return;

	switch (item->GetType())
	{
	case Equipment::EquipType::ARMOR:
		if (item->GetArmorProperties().armorType == ArmorProperties::ArmorType::HEADGEAR &&
				m_eEquipSlots != Stats::EquipSlots::HEADGEAR ||
			item->GetArmorProperties().armorType == ArmorProperties::ArmorType::CHEST_BODY &&
				m_eEquipSlots != Stats::EquipSlots::CHEST_BODY ||
			item->GetArmorProperties().armorType == ArmorProperties::ArmorType::FOOTWEAR &&
				m_eEquipSlots != Stats::EquipSlots::FOOTWEAR)
			return;
		break;
	case Equipment::EquipType::WEAPON:
		if (m_eEquipSlots != Stats::EquipSlots::WEAPON)
			return;
		break;
	case Equipment::EquipType::ACCESSORY:
		if (m_eEquipSlots != Stats::EquipSlots::ACCESSORY)
			return;
		break;
	case Equipment::EquipType::NO_TYPE: TRPG_LOG("THERE SHOULD NEVER BE NO TYPE FOR EQUIPMENT"); return;
	}

	const auto& name = item->GetName();
	m_Console.Write(x, y, name);

	const auto& index = m_EquipmentSelector.GetIndex();
	if (index < m_EquipmentSelector.GetData().size())
	{
		const auto& item_desc = m_EquipmentSelector.GetData()[ index ]->GetDescription();
		m_Console.Write(m_CenterScreenW - (item_desc.size() / 2),
						32,
						item_desc); // TODO: Center the description based on the half size and center
	}
}

void EquipmentMenuState::RenderEquipSlots(int x, int y, const std::wstring& item)
{
	if (item.empty())
		return;

	// Render the slot label
	m_Console.Write(x, y, item + L":");
	std::wstring equippedItem = L"";

	if (item == L"Weapon")
	{
		const auto& weapon = m_Player.GetEquippedItemsSlots()[ Stats::EquipSlots::WEAPON ];

		if (weapon)
			equippedItem = weapon->GetName();
	}
	else if (item == L"Armor")
	{
		const auto& armor = m_Player.GetEquippedItemsSlots()[ Stats::EquipSlots::CHEST_BODY ];

		if (armor)
			equippedItem = armor->GetName();
	}
	else if (item == L"Headgear")
	{
		const auto& head_gear = m_Player.GetEquippedItemsSlots()[ Stats::EquipSlots::HEADGEAR ];

		if (head_gear)
			equippedItem = head_gear->GetName();
	}
	else if (item == L"Footwear")
	{
		const auto& accessory = m_Player.GetEquippedItemsSlots()[ Stats::EquipSlots::FOOTWEAR ];

		if (accessory)
			equippedItem = accessory->GetName();
	}
	else if (item == L"Accessory")
	{
		const auto& accessory = m_Player.GetEquippedItemsSlots()[ Stats::EquipSlots::ACCESSORY ];

		if (accessory)
			equippedItem = accessory->GetName();
	}

	if (equippedItem.empty())
		equippedItem = L"Empty";

	m_Console.Write(x + 25, y, equippedItem);
}

void EquipmentMenuState::RemoveEquipment(int index, std::vector<std::wstring>& data)
{
	const auto& item = m_Player.GetEquippedItemsSlots()[ m_eEquipSlots ];

	if (!item)
		return;

	item->OnRemove(m_Player);
	m_Player.GetEquippedItemsSlots()[ m_eEquipSlots ] = nullptr;
	m_Console.ClearBuffer();
}

void EquipmentMenuState::UpdateIndex()
{
	auto index = m_EquipmentSelector.GetIndex();

	// Clear the description area
	if (m_PrevIndex != index)
	{
		m_PrevIndex = index;
		m_Console.ClearBuffer();
	}
}

EquipmentMenuState::EquipmentMenuState(Player& player, Console& console, StateMachine& stateMachine, Keyboard& keyboard)
	: m_Console(console)
	, m_StateMachine(stateMachine)
	, m_Keyboard(keyboard)
	, m_Player(player)
	, m_MenuSelector{console, keyboard, {L"EQUIP", L"REMOVE", L"OPTIMIZE"}, SelectorParams{42, 10, 3, 15, 2}

	  }
	, m_EquipSlotSelector{console,
						  keyboard,
						  std::bind(&EquipmentMenuState::OnSlotSelect, this, _1, _2),
						  std::bind(&EquipmentMenuState::RenderEquipSlots, this, _1, _2, _3),
						  player.GetEquipSlotLabels(),
						  SelectorParams{30, 15, 1, 2, 1}}
	, m_EquipmentSelector{console,
						  keyboard,
						  std::bind(&EquipmentMenuState::OnEquipSelect, this, _1, _2),
						  std::bind(&EquipmentMenuState::RenderEquip, this, _1, _2, _3),
						  player.GetInventory().GetEquipment(),
						  SelectorParams{30, 34, 3, 20, 2}}
	, m_bExitGame{false}
	, m_bInMenuSelect{true}
	, m_bInSlotSelect{false}
	, m_bRemoveEquipment{false}
	, m_ScreenWidth{console.GetScreenWidth()}
	, m_ScreenHeight{console.GetScreenHeight()}
	, m_CenterScreenW{console.GetHalfWidth()}
	, m_PanelBarX{m_CenterScreenW - (PANEL_BARS / 2)}
	, m_DiffPosY{0}
	, m_PrevStatModPos{0}
	, m_PrevIndex{-1}
	, m_sCurrentSlot{L"NO_SLOT"}
	, m_eEquipSlots{Stats::EquipSlots::NO_SLOT}
{
	m_MenuSelector.SetSelectionFunc(std::bind(&EquipmentMenuState::OnMenuSelect, this, _1, _2));
	m_EquipmentSelector.HideCursor();
	m_EquipSlotSelector.HideCursor();
}

EquipmentMenuState::~EquipmentMenuState()
{
}

void EquipmentMenuState::OnEnter()
{
	m_Console.ClearBuffer();
}

void EquipmentMenuState::OnExit()
{
	m_Console.ClearBuffer();
}

void EquipmentMenuState::Update()
{
	UpdateIndex();
}

void EquipmentMenuState::Draw()
{
	DrawEquipment();
	DrawPlayerInfo();

	m_MenuSelector.Draw();

	m_EquipSlotSelector.Draw();

	if (!m_bInSlotSelect && !m_bInMenuSelect)
	{
		DrawStatPrediction();
		m_EquipmentSelector.Draw();
	}

	m_Console.Draw();
}

void EquipmentMenuState::ProcessInputs()
{
	if (m_bInMenuSelect)
	{
		if (m_Keyboard.IsKeyJustPressed(KEY_BACKSPACE))
		{
			m_StateMachine.PopState();
			return;
		}

		m_MenuSelector.ProcessInputs();
	}
	else if (m_bInSlotSelect)
	{
		if (m_Keyboard.IsKeyJustPressed(KEY_BACKSPACE))
		{
			m_MenuSelector.ShowCursor();
			m_EquipSlotSelector.HideCursor();
			m_bInMenuSelect = true;
			m_bInSlotSelect = false;
			m_bRemoveEquipment = false;
			m_Console.ClearBuffer();
			return;
		}

		m_EquipSlotSelector.ProcessInputs();
	}
	else
	{
		if (m_Keyboard.IsKeyJustPressed(KEY_BACKSPACE))
		{
			m_EquipSlotSelector.ShowCursor();
			m_EquipmentSelector.HideCursor();
			m_bInMenuSelect = false;
			m_bInSlotSelect = true;
			m_Console.ClearBuffer();
			return;
		}

		m_EquipmentSelector.ProcessInputs();
	}
}

bool EquipmentMenuState::Exit()
{
	return false;
}
