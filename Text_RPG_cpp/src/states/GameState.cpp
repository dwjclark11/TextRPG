#include "GameState.h"
#include "StateMachine.h"
#include "../Logger.h"
#include "../inputs/Keyboard.h"

GameState::GameState(Keyboard& keyboard, StateMachine& stateMachine)
	: m_Keyboard(keyboard)
	, m_StateMachine(stateMachine)
{

}

GameState::~GameState()
{

}

void GameState::OnEnter()
{
	TRPG_LOG("ENTERED GAME STATE");
}

void GameState::OnExit()
{
	TRPG_LOG("EXIT GAME STATE");
}

void GameState::Update()
{
	TRPG_LOG("UPDATE GAME STATE");
}

void GameState::Draw()
{

}

void GameState::ProcessInputs()
{
	if (m_Keyboard.IsKeyJustPressed(KEY_ESCAPE))
	{
		m_StateMachine.PopState();
	}
}

bool GameState::Exit()
{
	return false;
}
