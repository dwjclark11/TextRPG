#include "GameMenuState.h"
#include "../Party.h"
#include "../Console.h"
#include "../inputs/Keyboard.h"
#include "StateMachine.h"

GameMenuState::GameMenuState(Party& party, Console& console, StateMachine& stateMachine, Keyboard& keyboard)
	: m_Party(party)
	, m_Console(console)
	, m_StateMachine(stateMachine)
	, m_Keyboard(keyboard)
{

}

GameMenuState::~GameMenuState()
{
}

void GameMenuState::OnEnter()
{
	m_Console.ClearBuffer();
}

void GameMenuState::OnExit()
{
	m_Console.ClearBuffer();
}

void GameMenuState::Update()
{
}

void GameMenuState::Draw()
{
	m_Console.Write(20, 10, L"WE ARE IN THE GAME MENU!", RED);
}

void GameMenuState::ProcessInputs()
{
	if (m_Keyboard.IsKeyJustPressed(KEY_BACKSPACE))
	{
		m_StateMachine.PopState();
		return;
	}
}

bool GameMenuState::Exit()
{
	return false;
}
