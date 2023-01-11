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
{
	m_TestPlayer = std::make_unique<Player>(L"Test Player", 1, 200);
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
	const auto& name = m_TestPlayer->GetName();
	std::wstring hp = std::to_wstring(m_TestPlayer->GetHP());
	std::wstring max_hp = std::to_wstring(m_TestPlayer->GetMaxHP());
	

	m_Console.Write(50, 30, name, BLUE);
	m_Console.Write(50, 32, L"HP: " + hp + L"/" + max_hp, BLUE);
	
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
