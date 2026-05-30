#include "Game.h"
#include "ui/Console.h"
#include "inputs/Keyboard.h"

#include "states/StateMachine.h"
#include "states/TitleState.h"

#include "utilities/Logger.h"
#include "utilities/Globals.h"

#include "actor/Party.h"

bool Game::Init()
{
	m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	m_pStateMachine->PushState(std::make_unique<TitleState>( m_Context ));

	return true;
}

void Game::ProcessEvents()
{
	// Get the number of Console Inputs
	if (!GetNumberOfConsoleInputEvents(m_hConsoleIn, &m_NumRead))
	{
		DWORD error = GetLastError();
		TRPG_ERROR("Failed to get number of console input events!" + error);
		return;
	}

	if (m_NumRead <= 0)
		return;

	if (!PeekConsoleInput(m_hConsoleIn, m_InRecBuf, 128, &m_NumRead))
	{
		DWORD error = GetLastError();
		TRPG_ERROR("Failed to Peek Events!" + error);
		return;
	}

	for (int i = 0; i < m_NumRead; i++)
	{
		switch (m_InRecBuf[ i ].EventType)
		{
		case KEY_EVENT: KeyEventProcess(m_InRecBuf[ i ].Event.KeyEvent); break;
		default: break;
		}
	}

	// Clear or Flush Other inputs
	FlushConsoleInputBuffer(m_hConsoleIn);
}

void Game::ProcessInputs()
{
	if (m_pKeyboard->IsKeyJustPressed(KEY_ESCAPE))
		m_bIsRunning = false;

	if (m_pStateMachine->Empty())
	{
		TRPG_ERROR("NO STATES IN STATE MACHINE TO PROCESS INPUTS!");
		m_bIsRunning = false;
		return;
	}

	m_pStateMachine->GetCurrentState()->ProcessInputs();
}

void Game::Update()
{
	if (m_pStateMachine->Empty())
	{
		TRPG_ERROR("NO STATES IN STATE MACHINE TO UPDATE!");
		m_bIsRunning = false;
		return;
	}

	m_pStateMachine->GetCurrentState()->Update();
	m_pKeyboard->Update();

	TRPG_Globals::GetInstance().Update();
}

void Game::Draw()
{
	if (m_pStateMachine->Empty())
	{
		TRPG_ERROR("NO STATES IN STATE MACHINE TO DRAW!");
		m_bIsRunning = false;
		return;
	}

	m_pStateMachine->GetCurrentState()->Draw();
	m_pConsole->Draw();
}

void Game::KeyEventProcess(KEY_EVENT_RECORD keyEvent)
{
	if (keyEvent.bKeyDown)
		m_pKeyboard->OnKeyDown(keyEvent.wVirtualKeyCode);
	else
		m_pKeyboard->OnKeyUp(keyEvent.wVirtualKeyCode);
}

Game::Game()
	: m_pConsole{ std::make_unique<Console>() }
	, m_pKeyboard{ std::make_unique<Keyboard>() }
	, m_pStateMachine{ std::make_unique<StateMachine>() }
	, m_pParty{ std::make_unique<Party>() }
	, m_Context{ .console = *m_pConsole, .stateMachine = *m_pStateMachine, .party = *m_pParty, .keyboard = *m_pKeyboard }
	, m_bIsRunning{ true }
{

}

Game::~Game()
{
}

void Game::Run()
{
	if (!Init())
		m_bIsRunning = false;

	while (m_bIsRunning)
	{
		ProcessEvents();
		ProcessInputs();

		Update();
		Draw();
	}
}
