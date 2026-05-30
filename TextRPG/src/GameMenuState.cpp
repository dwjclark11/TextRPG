#include "states/GameMenuState.h"
#include "states/ItemMenuState.h"
#include "states/EquipmentMenuState.h"
#include "states/StatusMenuState.h"
#include "states/StateMachine.h"

#include "actor/Party.h"
#include "actor/Player.h"
#include "ui/Console.h"
#include "inputs/Keyboard.h"

#include "utilities/Globals.h"

constexpr int PANEL_BARS = 90;
constexpr int MENU_SIZE = 27;
constexpr int CLEAR_ROW = 90;
constexpr int SMALL_PANEL_BAR = 29;

using namespace std::placeholders;

void GameMenuState::DrawPanels()
{
	// Draw Opening Bar
	m_Context.console.DrawPanelHorz(m_PanelBarX - 1, 1, PANEL_BARS + 2, BLUE);
	int menu_x_pos = m_CenterScreenW - (MENU_SIZE / 2);
	m_Context.console.Write(menu_x_pos, 2, L" __  __                  ", GREEN);
	m_Context.console.Write(menu_x_pos, 3, L"|  \\/  | ___ _ __  _   _ ", GREEN);
	m_Context.console.Write(menu_x_pos, 4, L"| |\\/| |/ _ \\ '_ \\| | | |", GREEN);
	m_Context.console.Write(menu_x_pos, 5, L"| |  | |  __/ | | | |_| |", GREEN);
	m_Context.console.Write(menu_x_pos, 6, L"|_|  |_|\\___|_| |_|\\__,_|", GREEN);
	m_Context.console.DrawPanelHorz(18, 7, PANEL_BARS, BLUE);

	m_Context.console.DrawPanelHorz(m_PanelBarX - 1, 34, SMALL_PANEL_BAR, BLUE);
	// Draw Closing Bar
	m_Context.console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 2), PANEL_BARS + 2, BLUE);

	// Draw Right bar
	m_Context.console.DrawPanelVert(m_PanelBarX - 1, 2, 44, BLUE);
	m_Context.console.DrawPanelVert(m_PanelBarX + PANEL_BARS, 2, 44, BLUE);
	m_Context.console.DrawPanelVert(48, 8, 38, BLUE);

	// Draw the game time
	const auto& time_str = L"TIME: " + TRPG_Globals::GetInstance().GetTime();
	m_Context.console.Write(26, 38, time_str);
}

void GameMenuState::DrawPlayerInfo()
{
	int i = 1;
	for (const auto& player : m_Context.party.GetParty())
	{
		if (!player->IsActive())
			continue;

		// Get the player attributes
		const auto& name = player->GetName();
		const auto& hp = std::to_wstring(player->GetHP());
		const auto& hp_max = std::to_wstring(player->GetMaxHP());
		const auto& level = std::to_wstring(player->GetLevel());
		const auto& xp = std::to_wstring(player->GetXP());
		const auto& xp_to_next = std::to_wstring(player->GetXPToNextLevel());

		std::wstring hp_string = L"HP: " + hp + L" / " + hp_max;
		std::wstring level_string = L"Lvl: " + level + L" Exp: " + xp + L" / " + xp_to_next;

		m_Context.console.Write(75, 12 + i, name, LIGHT_AQUA);
		m_Context.console.Write(75, 13 + i, hp_string, LIGHT_AQUA);
		m_Context.console.Write(75, 14 + i, level_string, LIGHT_AQUA);
		i += 10;
	}

	// Draw the party Gold
	const auto& gold = std::to_wstring(m_Context.party.GetGold());

	std::wstring gold_str = L"GOLD: " + gold;
	m_Context.console.Write(26, 36, gold_str);
}

void GameMenuState::OnMenuSelect(int index, std::vector<std::wstring> data)
{
	switch (index)
	{
	case 0: m_eSelectType = SelectType::ITEM; break;
	case 1: m_eSelectType = SelectType::MAGIC; break;
	case 2: m_eSelectType = SelectType::EQUIPMENT; break;
	case 3: m_eSelectType = SelectType::STATS; break;
	case 4: m_eSelectType = SelectType::ORDER; break;
	case 5:
		// TODO: Save Game
		return;
	case 6: m_bExitGame = true; return;
	default: return;
	}

	if (m_eSelectType != SelectType::NONE)
	{
		m_MenuSelector.HideCursor();
		m_PlayerSelector.ShowCursor();
		m_bInMenuSelect = false;
	}
}

void GameMenuState::OnPlayerSelect(int index, std::vector<std::shared_ptr<Player>> data)
{
	const auto& player = data[ index ];
	switch (m_eSelectType)
	{
	case SelectType::ITEM:
		m_Context.stateMachine.PushState(std::make_unique<ItemState>(m_Context, *player));
		break;
	case SelectType::MAGIC:
		// TODO: Create new MAGIC State
		break;
	case SelectType::EQUIPMENT:
		m_Context.stateMachine.PushState(std::make_unique<EquipmentMenuState>( *player, m_Context ));
		break;
	case SelectType::STATS:
		m_Context.stateMachine.PushState(std::make_unique<StatusMenuState>( *player, m_Context ));
		break;
	case SelectType::ORDER: SetOrderPlacement(player->GetPartyPosition()); break;
	default: break;
	}
}

void GameMenuState::OnDrawPlayerSelect(int x, int y, std::shared_ptr<Player> player)
{
	/*NOTHING TO DO HERE -- PLACEHOLDER*/
}

void GameMenuState::SetOrderPlacement(int playerPosition)
{
	if (m_FirstChoice < 0)
		m_FirstChoice = playerPosition;
	else if (m_SecondChoice < 0)
		m_SecondChoice = playerPosition;
}

void GameMenuState::UpdatePlayerOrder()
{
	if (m_FirstChoice < 0 || m_SecondChoice < 0)
		return;

	for (auto& member : m_Context.party.GetParty())
	{
		if (member->GetPartyPosition() == m_FirstChoice)
		{
			member->SetPartyPosition(m_SecondChoice);
		}
		else if (member->GetPartyPosition() == m_SecondChoice)
		{
			member->SetPartyPosition(m_FirstChoice);
		}
	}

	// sort the original data
	std::sort(m_Context.party.GetParty().begin(),
			  m_Context.party.GetParty().end(),
			  [ & ](std::shared_ptr<Player>& rh, std::shared_ptr<Player>& lh) {
				  return rh->GetPartyPosition() < lh->GetPartyPosition();
			  });

	// sort the selector data
	std::sort(m_PlayerSelector.GetData().begin(),
			  m_PlayerSelector.GetData().end(),
			  [ & ](std::shared_ptr<Player>& rh, std::shared_ptr<Player>& lh) {
				  return rh->GetPartyPosition() < lh->GetPartyPosition();
			  });

	m_FirstChoice = m_SecondChoice = -1;
	m_bInMenuSelect = true;
	m_eSelectType = SelectType::NONE;
	m_MenuSelector.ShowCursor();
	m_PlayerSelector.HideCursor();
	m_Context.console.ClearBuffer();
}

GameMenuState::GameMenuState(StateContext& context)
	: m_Context{ context }
	, m_MenuSelector{
		context.console,
		context.keyboard,
		{L"Items", L"Magic", L"Equipment", L"Stats", L"Order", L"Save", L"Exit"},
		SelectorParams{30, 8, 1, 0, 4}
	}
	, m_PlayerSelector{
		context.console,
		context.keyboard,
		std::bind(&GameMenuState::OnPlayerSelect, this, _1, _2),
		std::bind(&GameMenuState::OnDrawPlayerSelect, this, _1, _2, _3),
		context.party.GetParty(),
		SelectorParams{69, 13, 1, 0, 10}
	}
	, m_bExitGame{false}
	, m_bInMenuSelect{true}
	, m_ScreenWidth{ context.console.GetScreenWidth() }
	, m_ScreenHeight{ context.console.GetScreenHeight() }
	, m_CenterScreenW{ context.console.GetHalfWidth() }
	, m_PanelBarX{ m_CenterScreenW - (PANEL_BARS / 2) }
	, m_FirstChoice{-1}
	, m_SecondChoice{-1}
	, m_eSelectType{ SelectType::NONE }
{
	m_MenuSelector.SetSelectionFunc(std::bind(&GameMenuState::OnMenuSelect, this, _1, _2));
}

GameMenuState::~GameMenuState()
{
}

void GameMenuState::OnEnter()
{
	if (m_bInMenuSelect)
		m_PlayerSelector.HideCursor();

	m_Context.console.ClearBuffer();
}

void GameMenuState::OnExit()
{
	m_Context.console.ClearBuffer();
}

void GameMenuState::Update()
{
	UpdatePlayerOrder();
}

void GameMenuState::Draw()
{
	DrawPanels();
	DrawPlayerInfo();

	m_MenuSelector.Draw();
	m_PlayerSelector.Draw();

	m_Context.console.Draw();
}

void GameMenuState::ProcessInputs()
{
	if (m_bInMenuSelect)
	{
		if (m_Context.keyboard.IsKeyJustPressed(KEY_BACKSPACE))
		{
			m_Context.stateMachine.PopState();
			return;
		}

		m_MenuSelector.ProcessInputs();
	}
	else
	{
		if (m_Context.keyboard.IsKeyJustPressed(KEY_BACKSPACE))
		{
			m_PlayerSelector.HideCursor();
			m_bInMenuSelect = true;
			m_eSelectType = SelectType::NONE;
			m_FirstChoice = m_SecondChoice = -1;
			m_MenuSelector.ShowCursor();
			m_Context.console.ClearBuffer();
			return;
		}

		m_PlayerSelector.ProcessInputs();
	}
}

bool GameMenuState::Exit()
{
	return m_bExitGame;
}
