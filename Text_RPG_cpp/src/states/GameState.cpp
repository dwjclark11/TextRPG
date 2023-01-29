#include "GameState.h"
#include "StateMachine.h"
#include "../Logger.h"
#include "../Console.h"
#include "../inputs/Keyboard.h"

GameState::GameState(Console& console, Keyboard& keyboard, StateMachine& stateMachine)
	: m_Console(console)
	, m_Keyboard(keyboard)
	, m_StateMachine(stateMachine)
	, m_Selector(console, keyboard, {L"Start", L"Settings", L"Exit"})
	, m_Party{nullptr}
{
	m_Party = std::make_unique<Party>();

	auto player = std::make_shared<Player>(L"Test Player", L"test_player", m_Party->GetInventory(), 1, 200);

	m_Party->AddMember(std::move(player));
}

GameState::~GameState()
{

}

void GameState::OnEnter()
{
	m_Console.ClearBuffer();
}

void GameState::OnExit()
{
	m_Console.ClearBuffer();
}

void GameState::Update()
{
	
}

void GameState::Draw()
{
	// Test to draw the player stats
	for (const auto& member : m_Party->GetParty())
	{

		const auto& name = member->GetName();
		std::wstring hp = std::to_wstring(member->GetHP());
		std::wstring max_hp = std::to_wstring(member->GetMaxHP());

		m_Console.Write(50, 30, name, BLUE);
		m_Console.Write(50, 32, L"HP: " + hp + L"/" + max_hp, BLUE);

		const auto& stats_list = member->GetStats().GetStatList();
		// Draw the player attributes
		int i = 0;
		for (const auto& [stat, value] : stats_list)
		{
			const auto& mod_value = member->GetStats().GetModifier(stat);
			m_Console.Write(50, 34 + i, stat + L":");
			m_Console.Write(70, 34 + i, std::to_wstring(value + mod_value));
			i++;
		}
	}
	

	m_Selector.Draw();
	m_Console.Draw();
}

void GameState::ProcessInputs()
{
	if (m_Keyboard.IsKeyJustPressed(KEY_ESCAPE))
	{
		m_StateMachine.PopState();
		return;
	}

	m_Selector.ProcessInputs();
}

bool GameState::Exit()
{
	return false;
}
