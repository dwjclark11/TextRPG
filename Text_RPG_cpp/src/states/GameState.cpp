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
	m_Selector.Draw();
	m_Console.Draw();
}

void GameState::ProcessInputs()
{
	if (m_Keyboard.IsKeyJustPressed(KEY_ESCAPE))
	{
		m_StateMachine.PopState();
	}

	m_Selector.ProcessInputs();
}

bool GameState::Exit()
{
	return false;
}
