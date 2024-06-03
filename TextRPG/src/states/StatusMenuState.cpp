#include "StatusMenuState.h"
#include "../Console.h"
#include "../Player.h"
#include "../inputs/Keyboard.h"
#include "StateMachine.h"

#include <cassert>

void StatusMenuState::DrawStatusPanel()
{
	// Draw Opening Bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 1, PANEL_BARS + 2, YELLOW);
	int menu_x_pos = m_CenterScreenW - (STATUS_SIZE / 2);
	m_Console.Write(menu_x_pos, 2, L"  ____  _        _             ", GREEN);
	m_Console.Write(menu_x_pos, 3, L" / ___|| |_ __ _| |_ _   _ ___ ", GREEN);
	m_Console.Write(menu_x_pos, 4, L" \\___ \\| __/ _` | __| | | / __|", GREEN);
	m_Console.Write(menu_x_pos, 5, L"  ___) | || (_| | |_| |_| \\__ \\", GREEN);
	m_Console.Write(menu_x_pos, 6, L" |____/ \\__\\__,_|\\__|\\__,_|___/", GREEN);

	m_Console.DrawPanelHorz(m_PanelBarX - 1, 9, PANEL_BARS + 2, YELLOW);
	m_Console.DrawPanelHorz(m_PanelBarX - 1, 11, PANEL_BARS + 2, YELLOW);

	// Draw Closing Bar
	m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 2), PANEL_BARS + 2, YELLOW);

	// Draw Right bar
	m_Console.DrawPanelVert(m_PanelBarX - 1, 2, 44, YELLOW);
	m_Console.DrawPanelVert(m_PanelBarX + PANEL_BARS, 2, 44, YELLOW);
}

auto slot2str = [ & ](Stats::EquipSlots slot) {
	switch (slot)
	{
	case Stats::EquipSlots::WEAPON: return L"WEAPON: ";
	case Stats::EquipSlots::HEADGEAR: return L"HEADGEAR: ";
	case Stats::EquipSlots::CHEST_BODY: return L"CHEST: ";
	case Stats::EquipSlots::FOOTWEAR: return L"FOOTWEAR: ";
	case Stats::EquipSlots::ACCESSORY: return L"ACCESSORY: ";
	case Stats::EquipSlots::NO_SLOT: assert(false && "Should always have a slot!"); return L"NO_SLOT: ";
	default: assert(false && "Should always have a slot!"); return L"";
	}
};

void StatusMenuState::DrawPlayerInfo()
{
	const auto& player_name = m_Player.GetName();
	m_Console.Write(m_CenterScreenW - static_cast<int>(player_name.size() / 2), 10, player_name);

	const auto& level = std::to_wstring(m_Player.GetLevel());
	const auto& hp = std::to_wstring(m_Player.GetHP());
	const auto& hp_max = std::to_wstring(m_Player.GetMaxHP());

	const auto& mp = std::to_wstring(m_Player.GetMP());
	const auto& mp_max = std::to_wstring(m_Player.GetMaxMP());

	const auto& xp = std::to_wstring(m_Player.GetXP());
	const auto& xp2_next = std::to_wstring(m_Player.GetXPToNextLevel());

	const auto& level_str = L"LEVEL: " + level;
	const auto& hp_str = L"HP: " + hp + L" / " + hp_max;
	const auto& mp_str = L"MP: " + mp + L" / " + mp_max;
	const auto& xp_str = L"XP: " + xp + L" / " + xp2_next;

	m_Console.Write(STAT_LABEL_X_POS, 14, level_str);
	m_Console.Write(STAT_LABEL_X_POS, 15, hp_str);
	m_Console.Write(STAT_LABEL_X_POS, 16, mp_str);
	m_Console.Write(STAT_LABEL_X_POS, 17, xp_str);

	// Draw the equipment and slots
	m_Console.Write(STAT_LABEL_X_POS, 19, L"EQUIPMENT", YELLOW);
	m_Console.Write(STAT_LABEL_X_POS, 20, L"=========", YELLOW);

	int i = 0;
	const auto& equipment = m_Player.GetEquippedItemsSlots();

	for (const auto& [ slot, equip ] : equipment)
	{
		m_Console.Write(STAT_LABEL_X_POS, 21 + i, slot2str(slot));
		m_Console.Write(STAT_VAL_X_POS, 21 + i, equip ? equip->GetName() : L"EMPTY");
		i++;
	}
	i = 1;

	// Draw the equipment and slots
	m_Console.Write(STAT_LABEL_X_POS, STAT_LABEL_START_Y_POS - 2, L"ATTRIBUTES", YELLOW);
	m_Console.Write(STAT_LABEL_X_POS, STAT_LABEL_START_Y_POS - 1, L"==========", YELLOW);
	const auto& stat_list = m_Player.GetStats().GetStatList();
	for (const auto& [ stat, value ] : stat_list)
	{
		const auto& mod_value = m_Player.GetStats().GetModifier(stat);
		m_Console.Write(STAT_LABEL_X_POS, STAT_LABEL_START_Y_POS + i, stat);
		m_Console.Write(STAT_VAL_X_POS, STAT_LABEL_START_Y_POS + i, std::to_wstring(value + mod_value));
		i++;
	}
}

StatusMenuState::StatusMenuState(Player& player, Console& console, StateMachine& stateMachine, Keyboard& keyboard)
	: m_Player(player)
	, m_Console(console)
	, m_StateMachine(stateMachine)
	, m_Keyboard(keyboard)
	, m_ScreenWidth{console.GetScreenWidth()}
	, m_ScreenHeight{console.GetScreenHeight()}
	, m_CenterScreenW{console.GetHalfWidth()}
	, m_PanelBarX{m_CenterScreenW - (PANEL_BARS / 2)}
	, m_DiffPosY{0}
{
}

StatusMenuState::~StatusMenuState()
{
}

void StatusMenuState::OnEnter()
{
	m_Console.ClearBuffer();
}

void StatusMenuState::OnExit()
{
	m_Console.ClearBuffer();
}

void StatusMenuState::Update()
{
}

void StatusMenuState::Draw()
{
	DrawStatusPanel();
	DrawPlayerInfo();
	m_Console.Draw();
}

void StatusMenuState::ProcessInputs()
{
	if (m_Keyboard.IsKeyJustPressed(KEY_BACKSPACE))
	{
		m_StateMachine.PopState();
	}
}

bool StatusMenuState::Exit()
{
	return false;
}
